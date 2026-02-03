---
Theme: Public IP in Cloud
Date created: 2026-02-02 12:00
tags: [CloudLevel, Networking, Security, Accessibility]
---

## 📚 Idea/Concept

A Public IP address is a globally unique, Internet-routable address assigned to cloud resources, enabling direct communication with the Internet. In cloud environments, public IPs work in conjunction with Network Address Translation (NAT) and security controls to provide controlled external accessibility.

### Concept Overview

IP Address Types in Cloud:
```
┌─────────────────────────────────────────────────────────┐
│                    Internet                              │
│                        │                                 │
│              Public IP: 52.45.123.89                    │
│                        │                                 │
│                   ┌────▼────┐                           │
│                   │   NAT   │                           │
│                   └────┬────┘                           │
│                        │                                 │
│              Private IP: 10.0.1.45                      │
│                        │                                 │
│                   ┌────▼────┐                           │
│                   │   VM    │                           │
│                   └─────────┘                           │
│                                                         │
│                    VPC/VNet                             │
└─────────────────────────────────────────────────────────┘
```

### Address Types

Private IP Addresses:
- RFC 1918 ranges (10.x.x.x, 172.16-31.x.x, 192.168.x.x)
- Not routable on Internet
- Used within VPC for internal communication
- Automatically assigned from subnet CIDR

Public IP Addresses:
- Globally unique and Internet-routable
- Assigned from cloud provider's IP pool
- Subject to regional availability
- Incur costs when idle or attached

### Allocation Types

Dynamic Public IP:
- Assigned from pool when resource starts
- Released when resource stops/deallocates
- IP address may change on restart
- Lower cost, suitable for non-critical workloads

Static Public IP (Elastic/Reserved):
- Persists independently of resource lifecycle
- Can be remapped to different resources
- Essential for DNS records, allowlists, certificates
- Higher cost due to reservation

```
Dynamic IP Lifecycle:
VM Start  → IP Assigned (52.45.123.89)
VM Stop   → IP Released (back to pool)
VM Start  → IP Assigned (52.45.200.15)  ← Different!

Static IP Lifecycle:
Allocate  → IP Reserved (52.45.123.89)
VM Start  → IP Associated (52.45.123.89)
VM Stop   → IP Retained (52.45.123.89)   ← Same!
VM Start  → IP Associated (52.45.123.89)
```

### NAT (Network Address Translation)

Purpose:
- Translates between public and private addresses
- Conserves public IP address space
- Provides layer of abstraction/security

Types:
| NAT Type | Direction | Use Case |
|----------|-----------|----------|
| Source NAT (SNAT) | Outbound | Private resources accessing Internet |
| Destination NAT (DNAT) | Inbound | Internet accessing private resources |
| 1:1 NAT | Bidirectional | Static mapping for servers |

NAT Gateway Operation:
```
Outbound Request:
Private VM (10.0.1.45) → NAT GW → Internet
Source IP translated: 10.0.1.45 → 52.45.123.89

Inbound Response:
Internet → NAT GW → Private VM (10.0.1.45)
Destination IP translated: 52.45.123.89 → 10.0.1.45
```

### Security Considerations

Public IP ≠ Full Exposure:
Having a public IP does not mean unrestricted access. Security is controlled through:

1. Network Security Groups (NSG):
```
Rule: Allow inbound
- Port: 443 (HTTPS only)
- Source: 0.0.0.0/0 (any)
- Action: Allow

Rule: Deny all other
- Port: * (all)
- Source: 0.0.0.0/0
- Action: Deny
```

2. Web Application Firewall (WAF):
- Layer 7 protection
- OWASP rule sets
- Bot detection

3. DDoS Protection:
- Volumetric attack mitigation
- Protocol attack protection
- Application layer defense

### Common Use Cases

Resources Requiring Public IPs:
| Resource | Public IP Need | Alternative |
|----------|---------------|-------------|
| Web Server | Direct hosting | Load Balancer |
| API Gateway | External access | Cloud-managed API GW |
| VPN Gateway | Site-to-site | Direct Connect |
| Bastion Host | Admin access | Cloud-native bastion |
| Mail Server | SMTP | Managed email service |

### IP Address Management

Best Practices:
- Use static IPs for production services with DNS
- Tag and document IP allocations
- Monitor unused IPs (cost optimization)
- Plan IP strategy for multi-region deployments
- Consider IP reputation for email/sensitive services

DNS Integration:
```
Record Type: A
Name: api.example.com
Value: 52.45.123.89 (Static Public IP)
TTL: 300

For high availability:
Record Type: CNAME
Name: api.example.com
Value: api-lb.region.elb.amazonaws.com (Load Balancer DNS)
```

### Provider Terminology

| Concept | AWS | Azure | GCP |
|---------|-----|-------|-----|
| Static Public IP | Elastic IP | Static Public IP | External IP (static) |
| Dynamic Public IP | Public IP | Dynamic Public IP | External IP (ephemeral) |
| NAT Service | NAT Gateway | NAT Gateway | Cloud NAT |

### Cost Considerations

Billing factors:
- Idle Elastic/Static IPs (not attached)
- Data transfer through public IPs
- NAT Gateway processing charges
- Regional vs cross-region traffic

Optimization strategies:
- Release unused static IPs
- Use private endpoints where possible
- Consolidate through load balancers
- Consider PrivateLink for AWS services

## 📌 Key Points (Optional)
- Public IPs provide Internet accessibility but security is controlled by NSGs/firewalls.
- Static IPs persist across resource lifecycle; dynamic IPs change on restart.
- NAT enables private resources to access Internet without direct public exposure.

## 🖼️ Recommended Image
- Diagram showing NAT translation between public and private IPs with security group filtering.

## 🔗 Connections
- [[185-Virtual Private Cloud VPC]]
- [[183-IaaS vs PaaS vs SaaS]]
- [[191-Load Balancing in Cloud]]