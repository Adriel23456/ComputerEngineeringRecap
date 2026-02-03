---
Theme: Horizontal and Vertical Scaling in Cloud
Date created: 2026-02-02 12:00
tags: [CloudLevel, Scalability, HighAvailability, Performance]
---

## 📚 Idea/Concept

Scaling is the ability to adjust computing resources to meet demand. Vertical scaling (scale up) increases the power of individual instances, while horizontal scaling (scale out) adds more instances. Cloud-native architectures predominantly favor horizontal scaling for elasticity and resilience.

### Concept Overview

Visual Comparison:
```
Vertical Scaling (Scale Up):          Horizontal Scaling (Scale Out):
                                      
Before:  ┌───────┐                    Before:  ┌───┐
         │ Small │                             │ S │
         │  VM   │                             └───┘
         └───────┘                    
                                      After:   ┌───┐ ┌───┐ ┌───┐ ┌───┐
After:   ┌─────────────┐                       │ S │ │ S │ │ S │ │ S │
         │    Large    │                       └───┘ └───┘ └───┘ └───┘
         │     VM      │                             ▲
         │             │                       Load Balancer
         └─────────────┘              
```

### Vertical Scaling (Scale Up/Down)

Definition:
Increasing (or decreasing) the resources of a single instance by adding more CPU, RAM, storage, or network capacity.

Process:
```
1. Stop instance (downtime required)
2. Change instance type/size
3. Restart instance
4. Verify functionality

Timeline:
|--downtime--|
Stop → Resize → Start → Ready
```

Advantages:
- Simple implementation (no code changes)
- No distributed system complexity
- Works with any application architecture
- Single point of management

Disadvantages:
- Hardware limits exist (max instance size)
- Requires downtime for most operations
- Single point of failure remains
- Cost increases non-linearly
- Cannot scale infinitely

Instance Size Progression Example (AWS):
| Instance | vCPU | Memory | Network |
|----------|------|--------|---------|
| t3.micro | 2 | 1 GB | Low |
| t3.medium | 2 | 4 GB | Low-Mod |
| t3.large | 2 | 8 GB | Moderate |
| m5.xlarge | 4 | 16 GB | High |
| m5.4xlarge | 16 | 64 GB | High |
| m5.24xlarge | 96 | 384 GB | 25 Gbps |

Cost Non-Linearity:
```
Performance vs Cost (typical pattern):

Performance ▲
            │           ●●●●●
            │       ●●●●
            │    ●●●
            │  ●●
            │●●
            └─────────────────► Cost
            
Diminishing returns at higher tiers
```

### Horizontal Scaling (Scale Out/In)

Definition:
Adding (or removing) instances to distribute workload across multiple servers, using a load balancer to distribute traffic.

Process:
```
1. Provision new instance from template
2. Register with load balancer
3. Health check passes
4. Begin receiving traffic
5. No downtime required

Timeline:
──────────── Running ─────────────
         + New Instance
──────────── Running ─────────────
              │
        Both serving
```

Architecture Requirements:
```
┌─────────────────────────────────────────┐
│            Load Balancer                 │
│  (distributes traffic, health checks)   │
└─────────────────┬───────────────────────┘
                  │
    ┌─────────────┼─────────────┐
    │             │             │
    ▼             ▼             ▼
┌───────┐   ┌───────┐   ┌───────┐
│ App 1 │   │ App 2 │   │ App 3 │
│(state-│   │(state-│   │(state-│
│ less) │   │ less) │   │ less) │
└───────┘   └───────┘   └───────┘
    │             │             │
    └─────────────┼─────────────┘
                  │
                  ▼
         ┌───────────────┐
         │ Shared State  │
         │ (Database,    │
         │  Cache, etc.) │
         └───────────────┘
```

Advantages:
- No theoretical limit (add more instances)
- High availability (no SPOF)
- Zero-downtime scaling
- Better cost efficiency at scale
- Geographic distribution possible

Disadvantages:
- Requires stateless application design
- Distributed system complexity
- Need for load balancing
- Session management considerations
- Data consistency challenges

### Stateless Application Design

Critical Requirement for Horizontal Scaling:
Applications must be stateless - all state externalized to shared services.

```
Stateful (Cannot Scale Horizontally):
┌─────────────────────────┐
│       Application       │
│  ┌───────────────────┐ │
│  │  Session Data     │ │
│  │  User Cache       │ │
│  │  Temp Files       │ │
│  └───────────────────┘ │
└─────────────────────────┘

Stateless (Can Scale Horizontally):
┌─────────────────────────┐
│       Application       │
│  (no local state)       │
└───────────┬─────────────┘
            │
    ┌───────┴───────┐
    ▼               ▼
┌────────┐    ┌─────────┐
│ Redis  │    │  S3/    │
│ Cache  │    │ Storage │
└────────┘    └─────────┘
```

Externalized State Options:
| State Type | External Service |
|------------|------------------|
| Session | Redis, Memcached, DynamoDB |
| User Cache | Redis, ElastiCache |
| File Storage | S3, Blob Storage, GCS |
| Database | RDS, Aurora, Cloud SQL |
| Message Queue | SQS, RabbitMQ, Kafka |

### Auto Scaling

Automatic Horizontal Scaling based on metrics:

Configuration Components:
```
Auto Scaling Group:
├── Launch Template (what to create)
│   ├── AMI/Image
│   ├── Instance Type
│   ├── Security Groups
│   └── User Data (bootstrap)
│
├── Scaling Policies (when to scale)
│   ├── Target Tracking
│   │   └── Maintain CPU at 70%
│   ├── Step Scaling
│   │   └── If CPU > 80%, add 2
│   └── Scheduled
│       └── Scale up at 9 AM
│
└── Capacity Limits
    ├── Min: 2 (always running)
    ├── Max: 20 (cost control)
    └── Desired: 4 (normal state)
```

Common Scaling Metrics:
| Metric | Scale Out When | Scale In When |
|--------|---------------|---------------|
| CPU | > 70% | < 30% |
| Memory | > 80% | < 40% |
| Request Count | > 1000/min | < 200/min |
| Queue Depth | > 100 messages | < 10 messages |
| Custom | Business-specific | Business-specific |

Scaling Timeline:
```
Demand ▲
       │    ┌─────┐
       │   ╱       ╲
       │  ╱  Peak   ╲
       │ ╱           ╲
       │╱             ╲___________
       └──────────────────────────► Time
                                   
Instances ▲
          │    ┌──┐
          │   ╱    ╲
          │  ╱      ╲
          │ ╱        ╲___________
          │╱
          └──────────────────────► Time
          
Cooldown periods prevent thrashing
```

### Hybrid Approach

In practice, combining both strategies:

```
Strategy: Vertical scaling for baseline, horizontal for elasticity

┌─────────────────────────────────────────────────────┐
│                   Baseline Load                      │
│                                                      │
│  ┌───────────┐  ┌───────────┐  ┌───────────┐       │
│  │  Medium   │  │  Medium   │  │  Medium   │       │
│  │ Instance  │  │ Instance  │  │ Instance  │       │
│  └───────────┘  └───────────┘  └───────────┘       │
│                                                      │
│  Scaled vertically to handle normal load            │
└─────────────────────────────────────────────────────┘

              + Peak Demand →

┌─────────────────────────────────────────────────────┐
│                   Peak Load                          │
│                                                      │
│  ┌───────────┐  ┌───────────┐  ┌───────────┐       │
│  │  Medium   │  │  Medium   │  │  Medium   │       │
│  └───────────┘  └───────────┘  └───────────┘       │
│  ┌───────────┐  ┌───────────┐  ┌───────────┐       │
│  │  Medium   │  │  Medium   │  │  Medium   │       │
│  └───────────┘  └───────────┘  └───────────┘       │
│                                                      │
│  Scaled horizontally to handle peak                 │
└─────────────────────────────────────────────────────┘
```

### Comparison Summary

| Aspect | Vertical | Horizontal |
|--------|----------|------------|
| Complexity | Low | High |
| Downtime | Usually required | None |
| Max Capacity | Hardware limited | Virtually unlimited |
| SPOF Risk | Yes | No (with proper design) |
| Cost Model | Non-linear | Linear |
| Application Changes | None | Often required |
| Best For | Legacy apps, DBs | Cloud-native apps |

### Database Scaling Considerations

Databases present unique challenges:

Vertical Scaling (common for DBs):
- Simple, maintains ACID properties
- Limited by hardware
- Useful for OLTP workloads

Horizontal Scaling (read replicas):
```
         ┌─────────┐
         │ Primary │ ←── Writes
         │   DB    │
         └────┬────┘
              │ Replication
    ┌─────────┼─────────┐
    ▼         ▼         ▼
┌───────┐ ┌───────┐ ┌───────┐
│Replica│ │Replica│ │Replica│ ←── Reads
└───────┘ └───────┘ └───────┘
```

Sharding (true horizontal):
- Partition data across nodes
- Complex to implement
- Requires application changes
- Cross-shard queries are expensive

## 📌 Key Points (Optional)
- Vertical scaling is simpler but has hard limits and single point of failure.
- Horizontal scaling requires stateless design but offers better resilience and elasticity.
- Cloud-native applications should be designed for horizontal scaling from the start.
- Auto-scaling enables automatic response to demand changes.

## 🖼️ Recommended Image
- Side-by-side comparison showing vertical (bigger box) vs horizontal (more boxes) scaling with load balancer.

## 🔗 Connections
- [[185-Virtual Private Cloud VPC]]
- [[183-IaaS vs PaaS vs SaaS]]
- [[191-Load Balancing in Cloud]]
- [[193-Application Availability]]
- [[200-Containerization Fundamentals]]
- [[201-Kubernetes Container Orchestration]]
