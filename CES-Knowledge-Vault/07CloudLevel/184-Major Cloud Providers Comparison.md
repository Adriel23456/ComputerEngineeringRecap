---
Theme: Major Cloud Providers Comparison
Date created: 2026-02-02 12:00
tags: [CloudLevel, AWS, Azure, GCP, CloudStrategy, Fundamentals]
---

## 📚 Idea/Concept

The three major public cloud providers (AWS, Microsoft Azure, and Google Cloud Platform) dominate the market with overlapping but differentiated service offerings. Understanding their strengths, terminology differences, and ecosystem helps in making informed architectural decisions.

### Market Overview

Market Position (2025):
```
Cloud Market Share (IaaS + PaaS):

AWS    ████████████████████████████████ ~32%
Azure  █████████████████████████ ~23%
GCP    ███████████████ ~11%
Others ████████████████████████████████ ~34%
       (Alibaba, Oracle, IBM, etc.)

Revenue Scale: $200B+ combined annual revenue
Growth: ~20-25% year-over-year
```

### Service Comparison Matrix

Core Infrastructure Services:
| Service Category | AWS | Azure | GCP |
|-----------------|-----|-------|-----|
| **Compute (VMs)** | EC2 | Virtual Machines | Compute Engine |
| **Containers** | ECS, EKS | ACI, AKS | Cloud Run, GKE |
| **Serverless** | Lambda | Functions | Cloud Functions |
| **Object Storage** | S3 | Blob Storage | Cloud Storage |
| **Block Storage** | EBS | Managed Disks | Persistent Disk |
| **File Storage** | EFS | Azure Files | Filestore |
| **VPC/Network** | VPC | VNet | VPC |
| **Load Balancer** | ELB/ALB/NLB | Load Balancer | Cloud Load Balancing |
| **CDN** | CloudFront | Azure CDN | Cloud CDN |
| **DNS** | Route 53 | Azure DNS | Cloud DNS |

Database Services:
| Service Category | AWS | Azure | GCP |
|-----------------|-----|-------|-----|
| **Relational (Managed)** | RDS | Azure SQL | Cloud SQL |
| **PostgreSQL** | RDS PostgreSQL | Azure PostgreSQL | Cloud SQL PostgreSQL |
| **MySQL** | RDS MySQL | Azure MySQL | Cloud SQL MySQL |
| **NoSQL Document** | DynamoDB | Cosmos DB | Firestore |
| **NoSQL Key-Value** | DynamoDB | Table Storage | Bigtable |
| **In-Memory Cache** | ElastiCache | Azure Cache | Memorystore |
| **Data Warehouse** | Redshift | Synapse Analytics | BigQuery |

### Provider Strengths

```
┌─────────────────────────────────────────────────────────────────┐
│                         AWS                                      │
├─────────────────────────────────────────────────────────────────┤
│ ✓ Broadest service portfolio (200+ services)                    │
│ ✓ Most mature and battle-tested                                 │
│ ✓ Largest ecosystem (partners, marketplace)                     │
│ ✓ Most availability zones and regions                          │
│ ✓ Strong enterprise adoption                                    │
│                                                                  │
│ Best for: Complex architectures, enterprises, startups          │
│ Notable: Lambda (serverless pioneer), S3 (object storage std)   │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│                      Microsoft Azure                             │
├─────────────────────────────────────────────────────────────────┤
│ ✓ Best Microsoft/Windows integration                            │
│ ✓ Strong hybrid cloud (Azure Arc, Stack)                        │
│ ✓ Enterprise identity (Azure AD/Entra)                          │
│ ✓ Compliance certifications (government, healthcare)            │
│ ✓ Integrated with M365 and enterprise tools                     │
│                                                                  │
│ Best for: Microsoft shops, hybrid scenarios, enterprises        │
│ Notable: Azure AD, Power Platform, Teams integration            │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│                   Google Cloud Platform                          │
├─────────────────────────────────────────────────────────────────┤
│ ✓ Superior data analytics and ML/AI                             │
│ ✓ Best-in-class Kubernetes (GKE)                                │
│ ✓ Strong networking (Google's global network)                   │
│ ✓ BigQuery (serverless data warehouse)                          │
│ ✓ Open source friendly (Kubernetes, TensorFlow)                 │
│                                                                  │
│ Best for: Data-intensive, ML/AI, cloud-native startups          │
│ Notable: BigQuery, GKE, Vertex AI, global network               │
└─────────────────────────────────────────────────────────────────┘
```

### Terminology Mapping

Common concepts across providers:
```
┌─────────────────┬────────────────┬─────────────────┬────────────────┐
│    Concept      │      AWS       │     Azure       │      GCP       │
├─────────────────┼────────────────┼─────────────────┼────────────────┤
│ Account/Tenant  │ Account        │ Subscription    │ Project        │
│ Region          │ Region         │ Region          │ Region         │
│ Availability    │ Availability   │ Availability    │ Zone           │
│ Zone            │ Zone           │ Zone            │                │
│ Resource Group  │ (Tags/OUs)     │ Resource Group  │ (Labels/Proj)  │
│ IAM             │ IAM            │ Azure AD/Entra  │ IAM            │
│ CLI             │ aws-cli        │ az              │ gcloud         │
│ IaC Native      │ CloudFormation │ ARM/Bicep       │ Deployment Mgr │
│ Console         │ Console        │ Portal          │ Console        │
└─────────────────┴────────────────┴─────────────────┴────────────────┘
```

### Pricing Models

Common pricing approaches:
```
┌─────────────────────────────────────────────────────────────────┐
│                     Pricing Strategies                          │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  1. On-Demand / Pay-as-you-go                                   │
│     - No commitment, highest flexibility                        │
│     - Most expensive per-unit cost                              │
│                                                                  │
│  2. Reserved / Committed Use                                    │
│     AWS: Reserved Instances, Savings Plans                      │
│     Azure: Reserved VM Instances                                │
│     GCP: Committed Use Discounts                                │
│     - 1-3 year commitment                                       │
│     - 30-70% discount vs on-demand                              │
│                                                                  │
│  3. Spot / Preemptible                                          │
│     AWS: Spot Instances (up to 90% off)                        │
│     Azure: Spot VMs                                             │
│     GCP: Preemptible VMs, Spot VMs                             │
│     - Can be interrupted with short notice                      │
│     - Best for fault-tolerant workloads                         │
│                                                                  │
│  4. Sustained Use (GCP specific)                                │
│     - Automatic discounts for consistent usage                  │
│     - No commitment required                                    │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘

Cost Comparison (varies by region and config):
- Generally competitive; differences < 10-15% for similar configs
- Egress costs often the differentiator
- GCP often slightly cheaper for compute
- AWS/Azure have more complex pricing
```

### Decision Framework

Choosing a provider:
```
                        ┌─────────────────┐
                        │ Primary Driver? │
                        └────────┬────────┘
                                 │
        ┌────────────────────────┼────────────────────────┐
        │                        │                        │
        ▼                        ▼                        ▼
┌───────────────┐      ┌───────────────┐      ┌───────────────┐
│ Microsoft     │      │ Data/ML/AI    │      │ Broadest      │
│ Integration   │      │ Focus         │      │ Services      │
└───────┬───────┘      └───────┬───────┘      └───────┬───────┘
        │                      │                      │
        ▼                      ▼                      ▼
   ┌─────────┐           ┌─────────┐           ┌─────────┐
   │  Azure  │           │   GCP   │           │   AWS   │
   └─────────┘           └─────────┘           └─────────┘
```

Additional considerations:
| Factor | Recommendation |
|--------|----------------|
| Existing Microsoft stack | Azure |
| Heavy Kubernetes use | GCP (GKE is gold standard) |
| Largest service variety | AWS |
| Global network performance | GCP |
| Compliance-heavy (gov, health) | Azure or AWS GovCloud |
| Startup with credits | All offer programs |
| Multi-cloud strategy | Use Terraform, Kubernetes |

### Multi-Cloud Reality

Most organizations use multiple clouds:
```
Typical Enterprise Pattern:

┌─────────────────────────────────────────────────────────────────┐
│                                                                  │
│   Primary Cloud (70%)     Secondary (20%)      SaaS (10%)      │
│   ┌─────────────────┐    ┌─────────────────┐  ┌─────────────┐  │
│   │      AWS        │    │     Azure       │  │ Salesforce  │  │
│   │  - Core apps    │    │  - M365 integr  │  │ Workday     │  │
│   │  - Data lakes   │    │  - AD/Identity  │  │ ServiceNow  │  │
│   │  - Microservices│    │  - Some workload│  │             │  │
│   └─────────────────┘    └─────────────────┘  └─────────────┘  │
│                                                                  │
│   Unified by:                                                   │
│   - Kubernetes (portable workloads)                             │
│   - Terraform (infrastructure as code)                          │
│   - Central observability (Datadog, Splunk)                     │
│   - Identity federation                                         │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Avoiding Vendor Lock-in

Strategies:
```
┌─────────────────────────────────────────────────────────────────┐
│                    Portability Strategies                        │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  HIGH PORTABILITY:                                              │
│  ✓ Containers + Kubernetes (runs anywhere)                      │
│  ✓ Terraform (multi-cloud IaC)                                  │
│  ✓ PostgreSQL, MySQL (standard databases)                       │
│  ✓ Kafka (vs proprietary queues)                                │
│  ✓ S3-compatible storage APIs                                   │
│                                                                  │
│  MODERATE PORTABILITY:                                          │
│  ~ Managed Kubernetes (EKS/AKS/GKE)                             │
│  ~ Standard SQL databases                                       │
│  ~ Standard protocols (HTTPS, gRPC)                             │
│                                                                  │
│  LOW PORTABILITY (Lock-in risk):                                │
│  ✗ Proprietary serverless (Lambda, Functions)                   │
│  ✗ Provider-specific databases (DynamoDB, Cosmos, Spanner)      │
│  ✗ Proprietary ML platforms                                     │
│  ✗ Provider-specific SDKs deeply integrated                     │
│                                                                  │
│  Trade-off: Lock-in vs. Managed Service Benefits                │
└─────────────────────────────────────────────────────────────────┘
```

## 📌 Key Points (Optional)
- AWS leads in breadth, Azure in enterprise/Microsoft integration, GCP in data/ML.
- Most enterprises adopt multi-cloud strategies intentionally or organically.
- Kubernetes and Terraform provide cross-cloud portability.
- Lock-in trade-offs should be conscious architectural decisions.

## 🖼️ Recommended Image
- Three-column comparison chart with provider logos and key differentiators.

## 🔗 Connections
- [[183-IaaS vs PaaS vs SaaS]]
- [[192-Hybrid Cloud Architecture]]
- [[201-Kubernetes Container Orchestration]]
- [[185-Virtual Private Cloud VPC]]