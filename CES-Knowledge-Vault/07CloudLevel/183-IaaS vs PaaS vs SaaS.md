---
Theme: IaaS vs PaaS vs SaaS
Date created: 2026-02-02 12:00
tags: [CloudLevel, ServiceModels, CloudStrategy, Fundamentals]
---

## 📚 Idea/Concept

Cloud service models define the division of responsibility between cloud provider and customer across the technology stack. IaaS, PaaS, and SaaS represent progressively higher levels of abstraction, each trading control for convenience.

### Concept Overview

Responsibility Stack:
```
┌─────────────┬─────────────┬─────────────┬─────────────┐
│  On-Premise │    IaaS     │    PaaS     │    SaaS     │
├─────────────┼─────────────┼─────────────┼─────────────┤
│ Application │ Application │ Application │ ▓▓▓▓▓▓▓▓▓▓▓ │
│    Data     │    Data     │    Data     │ ▓▓▓▓▓▓▓▓▓▓▓ │
│   Runtime   │   Runtime   │ ▓▓▓▓▓▓▓▓▓▓▓ │ ▓▓▓▓▓▓▓▓▓▓▓ │
│  Middleware │  Middleware │ ▓▓▓▓▓▓▓▓▓▓▓ │ ▓▓▓▓▓▓▓▓▓▓▓ │
│     O/S     │     O/S     │ ▓▓▓▓▓▓▓▓▓▓▓ │ ▓▓▓▓▓▓▓▓▓▓▓ │
│ Virtualizn  │ ▓▓▓▓▓▓▓▓▓▓▓ │ ▓▓▓▓▓▓▓▓▓▓▓ │ ▓▓▓▓▓▓▓▓▓▓▓ │
│   Servers   │ ▓▓▓▓▓▓▓▓▓▓▓ │ ▓▓▓▓▓▓▓▓▓▓▓ │ ▓▓▓▓▓▓▓▓▓▓▓ │
│   Storage   │ ▓▓▓▓▓▓▓▓▓▓▓ │ ▓▓▓▓▓▓▓▓▓▓▓ │ ▓▓▓▓▓▓▓▓▓▓▓ │
│  Networking │ ▓▓▓▓▓▓▓▓▓▓▓ │ ▓▓▓▓▓▓▓▓▓▓▓ │ ▓▓▓▓▓▓▓▓▓▓▓ │
└─────────────┴─────────────┴─────────────┴─────────────┘
  □ = Customer manages    ▓ = Provider manages
```

### IaaS - Infrastructure as a Service

Definition:
Provider delivers fundamental computing resources (VMs, storage, networks) on-demand. Customer has full control over operating system and above.

Provider Manages:
- Physical data centers
- Networking infrastructure
- Storage hardware
- Virtualization layer
- Hardware maintenance

Customer Manages:
- Operating system (patching, security)
- Middleware and runtime
- Applications
- Data
- Security configuration

Examples:
| Provider | IaaS Services |
|----------|---------------|
| AWS | EC2, EBS, VPC |
| Azure | Virtual Machines, Managed Disks |
| GCP | Compute Engine, Persistent Disk |

Use Cases:
- Lift-and-shift migrations
- Development/test environments
- High-performance computing
- Custom infrastructure requirements
- Full control needed for compliance

Characteristics:
```
Flexibility:     ████████████ High
Control:         ████████████ High
Operational Overhead: ████████████ High
Vendor Lock-in:  ████░░░░░░░░ Low
Time to Deploy:  ████████░░░░ Medium
```

### PaaS - Platform as a Service

Definition:
Provider delivers a complete development and deployment environment. Customer focuses only on application code and data.

Provider Manages:
- Everything in IaaS, plus:
- Operating system
- Middleware
- Runtime environment
- Development tools
- Database management systems

Customer Manages:
- Application code
- Application configuration
- Data
- User access

Examples:
| Provider | PaaS Services |
|----------|---------------|
| AWS | Elastic Beanstalk, App Runner, Lambda |
| Azure | App Service, Azure Functions |
| GCP | App Engine, Cloud Run, Cloud Functions |
| Others | Heroku, Vercel, Railway |

Use Cases:
- Rapid application development
- Microservices deployment
- API backends
- Web applications
- Serverless computing

Characteristics:
```
Flexibility:     ████████░░░░ Medium
Control:         ████████░░░░ Medium
Operational Overhead: ████░░░░░░░░ Low
Vendor Lock-in:  ████████░░░░ Medium
Time to Deploy:  ████████████ Fast
```

### SaaS - Software as a Service

Definition:
Provider delivers complete, ready-to-use applications over the Internet. Customer simply uses the software via browser or API.

Provider Manages:
- Entire technology stack
- Application updates
- Security and compliance
- Availability and scaling
- Data backups

Customer Manages:
- User configuration
- Data input
- User access management
- Integration with other systems

Examples:
| Category | SaaS Examples |
|----------|---------------|
| Productivity | Microsoft 365, Google Workspace |
| CRM | Salesforce, HubSpot |
| Communication | Slack, Zoom, Teams |
| DevOps | GitHub, GitLab, Jira |
| Finance | QuickBooks, Stripe |

Use Cases:
- Business productivity tools
- Customer relationship management
- Human resources systems
- Collaboration platforms
- Non-differentiating business functions

Characteristics:
```
Flexibility:     ████░░░░░░░░ Low
Control:         ████░░░░░░░░ Low
Operational Overhead: ░░░░░░░░░░░░ Minimal
Vendor Lock-in:  ████████████ High
Time to Deploy:  ████████████ Immediate
```

### Detailed Comparison

| Aspect | IaaS | PaaS | SaaS |
|--------|------|------|------|
| Target User | IT Admins | Developers | End Users |
| Billing Model | Compute/Storage | Request/Runtime | Subscription/Seat |
| Scaling | Manual/Auto | Automatic | Transparent |
| Customization | Full | Application-level | Configuration only |
| Maintenance | High | Medium | None |
| Migration Effort | Low | Medium | High |

### Shared Responsibility Model

Security responsibility varies by model:
```
                    IaaS         PaaS         SaaS
┌──────────────────────────────────────────────────────┐
│ Data Classification    Customer     Customer    Customer │
│ Identity Management    Customer     Customer    Shared   │
│ Application Security   Customer     Customer    Provider │
│ Network Controls       Shared       Provider    Provider │
│ OS Patching           Customer     Provider    Provider │
│ Physical Security     Provider     Provider    Provider │
└──────────────────────────────────────────────────────┘
```

### Decision Framework

Choose IaaS when:
- Need full infrastructure control
- Running legacy applications
- Specific OS/kernel requirements
- Regulatory requirements mandate control
- Building custom platforms

Choose PaaS when:
- Focus on application development
- Need rapid deployment cycles
- Standard runtime requirements
- Want managed scaling
- Building cloud-native applications

Choose SaaS when:
- Standard business functionality needed
- Minimize IT overhead
- Quick time-to-value required
- Non-core business function
- Limited IT resources

### Emerging Models

Beyond the big three:
| Model | Description | Examples |
|-------|-------------|----------|
| FaaS | Function as a Service | Lambda, Functions |
| CaaS | Container as a Service | ECS, AKS, GKE |
| DBaaS | Database as a Service | RDS, CosmosDB |
| BaaS | Backend as a Service | Firebase, Supabase |

### Trade-offs Summary

```
Control ◄─────────────────────────────► Convenience
IaaS ────────── PaaS ────────── SaaS

Lock-in Risk ◄─────────────────────────► Operational Burden
SaaS ────────── PaaS ────────── IaaS

Cost Predictability ◄──────────────────► Cost Optimization
SaaS ────────── PaaS ────────── IaaS
```

## 📌 Key Points (Optional)
- IaaS provides maximum control with maximum operational responsibility.
- PaaS accelerates development by abstracting infrastructure management.
- SaaS delivers ready-to-use applications with minimal customer management.
- The choice involves trade-offs between control, convenience, and vendor lock-in.

## 🖼️ Recommended Image
- Stacked responsibility diagram showing customer vs provider responsibilities across all three models.

## 🔗 Connections
- [[185-Virtual Private Cloud VPC]]
- [[192-Hybrid Cloud Architecture]]
- [[190-Horizontal and Vertical Scaling in Cloud]]
- [[200-Containerization Fundamentals]]
- [[205-Serverless Computing]]
