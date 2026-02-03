---
Theme: Zone Redundant Storage ZRS
Date created: 2026-02-02 12:00
tags: [CloudLevel, Storage, Redundancy, AvailabilityZones, HighAvailability]
---

## 📚 Idea/Concept

Zone Redundant Storage (ZRS) automatically replicates data synchronously across multiple Availability Zones within a single region, providing protection against data center failures while maintaining low latency and strong consistency.

### Concept Overview

ZRS Architecture:
```
┌─────────────────────────────────────────────────────────────────┐
│                         Single Region                            │
│                                                                  │
│  Availability Zone 1   Availability Zone 2   Availability Zone 3│
│  ┌─────────────────┐   ┌─────────────────┐   ┌─────────────────┐│
│  │   Data Center   │   │   Data Center   │   │   Data Center   ││
│  │                 │   │                 │   │                 ││
│  │   ┌─────────┐   │   │   ┌─────────┐   │   │   ┌─────────┐   ││
│  │   │  Copy   │   │   │   │  Copy   │   │   │   │  Copy   │   ││
│  │   │   1     │◄──┼───┼──►│   2     │◄──┼───┼──►│   3     │   ││
│  │   └─────────┘   │   │   └─────────┘   │   │   └─────────┘   ││
│  │                 │   │                 │   │                 ││
│  │  Independent:   │   │  Independent:   │   │  Independent:   ││
│  │  - Power        │   │  - Power        │   │  - Power        ││
│  │  - Cooling      │   │  - Cooling      │   │  - Cooling      ││
│  │  - Networking   │   │  - Networking   │   │  - Networking   ││
│  └─────────────────┘   └─────────────────┘   └─────────────────┘│
│                                                                  │
│  Physical separation: tens of kilometers                        │
│  Network latency: < 2ms between zones                           │
└─────────────────────────────────────────────────────────────────┘
```

### Availability Zones Explained

What is an Availability Zone:
```
Region (e.g., US East)
├── Availability Zone 1
│   └── One or more data centers
│       ├── Independent power supply
│       ├── Independent cooling
│       ├── Independent networking
│       └── Physical security
│
├── Availability Zone 2
│   └── Separate physical location
│       └── (Same independent infrastructure)
│
└── Availability Zone 3
    └── Separate physical location
        └── (Same independent infrastructure)

Key Properties:
- Physically separated (mitigate correlated failures)
- Connected via high-bandwidth, low-latency network
- Designed for independent failure modes
```

### Replication Mechanism

Synchronous Write Across Zones:
```
Client Write Request
        │
        ▼
┌───────────────────┐
│  Storage Gateway  │
└─────────┬─────────┘
          │
    ┌─────┴─────┬─────────────┐
    │           │             │
    ▼           ▼             ▼
┌───────┐   ┌───────┐   ┌───────┐
│ AZ-1  │   │ AZ-2  │   │ AZ-3  │
│ Copy  │   │ Copy  │   │ Copy  │
└───────┘   └───────┘   └───────┘
    │           │             │
    └───────────┴─────────────┘
                │
         All 3 confirmed
         (synchronous)
                │
                ▼
      Acknowledge to Client

Write committed only after confirmation
from all availability zones
```

Consistency guarantee:
- Strong consistency (read-after-write)
- No eventual consistency window
- All zones have identical data after write acknowledgment

### Failure Scenarios

ZRS Resilience:
```
Scenario 1: Single AZ Failure
┌─────┐   ┌─────┐   ┌─────┐
│ AZ1 │   │ AZ2 │   │ AZ3 │
│  X  │   │ ✓   │   │ ✓   │
└─────┘   └─────┘   └─────┘
Result: Service continues, data safe

Scenario 2: Two AZ Failure (rare)
┌─────┐   ┌─────┐   ┌─────┐
│ AZ1 │   │ AZ2 │   │ AZ3 │
│  X  │   │  X  │   │ ✓   │
└─────┘   └─────┘   └─────┘
Result: Read-only mode possible, data safe

Scenario 3: Regional Failure
┌─────────────────────────────┐
│    Entire Region Down       │
│  ┌─────┐ ┌─────┐ ┌─────┐   │
│  │ AZ1 │ │ AZ2 │ │ AZ3 │   │
│  │  X  │ │  X  │ │  X  │   │
│  └─────┘ └─────┘ └─────┘   │
└─────────────────────────────┘
Result: Service unavailable, data at risk
(This is why GRS exists)
```

### Comparison with LRS

| Aspect | LRS | ZRS |
|--------|-----|-----|
| Copies | 3 within datacenter | 3 across AZs |
| Datacenter failure | Data at risk | Protected |
| Regional failure | Data at risk | Data at risk |
| Latency | Lowest | Very low (< 2ms) |
| Cost | Baseline | ~25% more |
| Durability | 11 nines (local) | 12 nines |
| Availability SLA | 99.9% | 99.9%+ |

### Durability and Availability Metrics

ZRS specifications:
```
Durability: 99.9999999999% (12 nines)
- Over 1 year with 1 million objects
- Expected loss: 0.000000000001 objects

Availability: 99.9% to 99.99%
- Annual downtime: 52 minutes to 8.76 hours
- Depends on specific service SLA

Comparison visualization:

           Durability (nines)
LRS        ████████████ 11
ZRS        █████████████ 12  
GRS        █████████████████ 16
```

### Use Cases

Ideal for ZRS:
| Scenario | Rationale |
|----------|-----------|
| Production workloads | Business continuity |
| Stateful applications | Data persistence critical |
| Real-time analytics | Cannot tolerate data center outage |
| Active-active deployments | Multi-AZ application architecture |
| Compliance (regional) | Data residency with HA |

When ZRS is not enough:
| Scenario | Better Option |
|----------|---------------|
| Cross-region DR | GRS |
| Global applications | GZRS or multi-region |
| Regulatory (geographic) | GRS with specific regions |

### Cost-Benefit Analysis

```
Protection Level vs Cost:

                    ┌─────────────────────────────────┐
                    │     Regional Disaster           │
                    │     Protection                  │
                    │            GRS ●                │
                    │               ╱                 │
    Protection      │              ╱                  │
    Level           │             ╱                   │
                    │        ZRS ●                    │
                    │           ╱                     │
                    │          ╱                      │
                    │     LRS ●                       │
                    │        ╱                        │
                    └─────────────────────────────────┘
                              Cost →

Sweet spot for most production workloads: ZRS
- Protects against most common failures
- Reasonable cost increase
- No cross-region complexity
```

### Multi-AZ Application Architecture

Combining ZRS with application design:
```
┌─────────────────────────────────────────────────────────┐
│                       Region                             │
│                                                          │
│  ┌──────────────────────────────────────────────────┐   │
│  │                 Load Balancer                     │   │
│  │              (Multi-AZ aware)                     │   │
│  └─────────────────────┬────────────────────────────┘   │
│                        │                                 │
│      ┌─────────────────┼─────────────────┐              │
│      │                 │                 │              │
│      ▼                 ▼                 ▼              │
│  ┌───────┐        ┌───────┐        ┌───────┐           │
│  │ App   │        │ App   │        │ App   │           │
│  │ AZ-1  │        │ AZ-2  │        │ AZ-3  │           │
│  └───┬───┘        └───┬───┘        └───┬───┘           │
│      │                │                │                │
│      └────────────────┼────────────────┘                │
│                       ▼                                  │
│              ┌─────────────────┐                        │
│              │   ZRS Storage   │                        │
│              │ (Replicated to  │                        │
│              │   all 3 AZs)    │                        │
│              └─────────────────┘                        │
│                                                          │
└─────────────────────────────────────────────────────────┘
```

### Provider Implementations

| Provider | ZRS Equivalent | Notes |
|----------|----------------|-------|
| Azure | ZRS | Explicit configuration |
| AWS | Standard (multi-AZ default) | S3 default behavior |
| GCP | Multi-region (closest) | Regional with replication |

## 📌 Key Points (Optional)
- ZRS replicates synchronously across 3 Availability Zones within a region.
- Protects against data center failures while maintaining low latency.
- Does not protect against regional disasters (use GRS for that).
- Recommended baseline for production workloads requiring high availability.

## 🖼️ Recommended Image
- Three data centers in different locations within region, connected by network lines showing synchronous replication.

## 🔗 Connections
- [[187-Locally Redundant Storage LRS]]
- [[189-Geo Redundant Storage GRS]]
- [[185-Virtual Private Cloud VPC]]
- [[193-Application Availability]]
- [[190-Horizontal and Vertical Scaling in Cloud]]