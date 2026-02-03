---
Theme: Virtual Private Cloud VPC
Date created: 2026-02-02 12:00
tags: [CloudLevel, Networking, Security, Infrastructure]
---

## 📚 Idea/Concept

A Virtual Private Cloud (VPC) is a logically isolated, software-defined network within a public cloud provider's infrastructure that gives you complete control over your virtual networking environment, including IP address ranges, subnets, route tables, and network gateways.

### Concept Overview

Traditional Network vs VPC:
```
Traditional Data Center:
┌─────────────────────────────────────────┐
│  Physical routers, switches, firewalls  │
│  Hardware procurement, maintenance      │
│  Fixed capacity, long provisioning      │
└─────────────────────────────────────────┘

Virtual Private Cloud:
┌─────────────────────────────────────────┐
│  Software-defined networking            │
│  API-driven configuration               │
│  Elastic capacity, instant provisioning │
│  Complete tenant isolation              │
└─────────────────────────────────────────┘
```

### Core Components

Subnets:
- Subdivisions of VPC IP address range
- Can be designated as public or private
- Each subnet resides in a single Availability Zone
- CIDR block defines the IP range (e.g., 10.0.1.0/24)

```
VPC: 10.0.0.0/16 (65,536 IPs)
├── Public Subnet A:  10.0.1.0/24 (256 IPs) - AZ-1
├── Public Subnet B:  10.0.2.0/24 (256 IPs) - AZ-2
├── Private Subnet A: 10.0.10.0/24 (256 IPs) - AZ-1
├── Private Subnet B: 10.0.11.0/24 (256 IPs) - AZ-2
└── Database Subnet:  10.0.20.0/24 (256 IPs) - AZ-1
```

Route Tables:
- Define rules for directing network traffic
- Each subnet must be associated with a route table
- Control traffic flow between subnets and external networks

| Destination | Target | Purpose |
|-------------|--------|---------|
| 10.0.0.0/16 | local | Internal VPC traffic |
| 0.0.0.0/0 | igw-xxx | Internet access (public) |
| 0.0.0.0/0 | nat-xxx | Internet access (private) |

### Network Gateways

Internet Gateway (IGW):
- Enables communication between VPC and Internet
- Horizontally scaled, redundant, highly available
- One IGW per VPC

NAT Gateway:
- Allows private subnet resources to access Internet
- Prevents inbound connections from Internet
- Deployed per Availability Zone for HA

```
Internet
    │
    ▼
┌───────────────┐
│ Internet GW   │
└───────────────┘
    │
    ├──────────────────────┐
    ▼                      ▼
┌──────────┐         ┌──────────┐
│ Public   │         │ NAT GW   │
│ Subnet   │         └──────────┘
│ (Web)    │              │
└──────────┘              ▼
                    ┌──────────┐
                    │ Private  │
                    │ Subnet   │
                    │ (App/DB) │
                    └──────────┘
```

### Security Controls

Network Security Groups (NSG) / Security Groups:
- Virtual firewalls at instance/resource level
- Stateful: return traffic automatically allowed
- Define inbound and outbound rules

```
Security Group: web-servers-sg
┌─────────────────────────────────────────────────────┐
│ Inbound Rules:                                      │
│   HTTP  (80)   from 0.0.0.0/0                      │
│   HTTPS (443)  from 0.0.0.0/0                      │
│   SSH   (22)   from 10.0.0.0/16 (internal only)   │
│                                                     │
│ Outbound Rules:                                     │
│   All traffic to 0.0.0.0/0 (default)              │
└─────────────────────────────────────────────────────┘
```

Network Access Control Lists (NACLs):
- Subnet-level firewall
- Stateless: must define both inbound and outbound
- Numbered rules evaluated in order

### VPC Connectivity Options

VPC Peering:
- Direct network connection between two VPCs
- Can span regions and accounts
- Non-transitive (A↔B and B↔C doesn't mean A↔C)

VPN Gateway:
- Encrypted tunnel to on-premises network
- Uses IPsec protocol
- Bandwidth limited by Internet connection

Direct Connect / ExpressRoute / Interconnect:
- Dedicated private connection to cloud provider
- Consistent network performance
- Higher bandwidth, lower latency than VPN

Transit Gateway:
- Hub for connecting multiple VPCs and on-premises
- Simplifies network topology
- Centralized routing management

```
              ┌─────────────┐
              │   Transit   │
              │   Gateway   │
              └──────┬──────┘
       ┌──────┬──────┼──────┬──────┐
       ▼      ▼      ▼      ▼      ▼
    VPC-A  VPC-B  VPC-C  VPN    Direct
                              Connect
```

### Multi-Tier Architecture Example

```
┌─────────────────────────────────────────────────────────┐
│                        VPC                               │
│  ┌───────────────────────────────────────────────────┐  │
│  │              Public Subnets (DMZ)                 │  │
│  │   ┌─────────┐  ┌─────────┐  ┌─────────┐         │  │
│  │   │   ALB   │  │   ALB   │  │ Bastion │         │  │
│  │   └─────────┘  └─────────┘  └─────────┘         │  │
│  └───────────────────────────────────────────────────┘  │
│  ┌───────────────────────────────────────────────────┐  │
│  │              Private Subnets (App Tier)           │  │
│  │   ┌─────────┐  ┌─────────┐  ┌─────────┐         │  │
│  │   │  App 1  │  │  App 2  │  │  App 3  │         │  │
│  │   └─────────┘  └─────────┘  └─────────┘         │  │
│  └───────────────────────────────────────────────────┘  │
│  ┌───────────────────────────────────────────────────┐  │
│  │              Private Subnets (Data Tier)          │  │
│  │   ┌─────────┐  ┌─────────┐  ┌─────────┐         │  │
│  │   │   DB    │  │  Cache  │  │  Queue  │         │  │
│  │   │ Primary │  │ Cluster │  │ Service │         │  │
│  │   └─────────┘  └─────────┘  └─────────┘         │  │
│  └───────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────┘
```

### CIDR Planning Best Practices

Considerations:
- Avoid overlapping with on-premises networks
- Plan for growth (don't use /28 for production)
- Reserve space for future subnets
- Consider multi-region expansion

Recommended sizing:
| Environment | VPC CIDR | Rationale |
|-------------|----------|-----------|
| Development | /20 | ~4,000 IPs, sufficient for dev |
| Production | /16 | ~65,000 IPs, room to grow |
| Enterprise | Multiple /16 | Segmentation by BU/app |

### Provider Terminology Mapping

| Concept | AWS | Azure | GCP |
|---------|-----|-------|-----|
| VPC | VPC | VNet | VPC |
| Subnet | Subnet | Subnet | Subnet |
| Security Group | Security Group | NSG | Firewall Rules |
| Route Table | Route Table | Route Table | Routes |
| NAT | NAT Gateway | NAT Gateway | Cloud NAT |
| Peering | VPC Peering | VNet Peering | VPC Peering |

## 📌 Key Points (Optional)
- VPC provides complete network isolation within shared cloud infrastructure.
- Proper subnet design (public/private) is foundational for security architecture.
- Security Groups and NACLs provide defense-in-depth at different levels.

## 🖼️ Recommended Image
- Multi-tier VPC architecture diagram showing public/private subnets across availability zones.

## 🔗 Connections
- [[186-Public IP in Cloud]]
- [[183-IaaS vs PaaS vs SaaS]]
- [[192-Hybrid Cloud Architecture]]
- [[188-Zone Redundant Storage ZRS]]