---
Theme: Locally Redundant Storage LRS
Date created: 2026-02-02 12:00
tags: [CloudLevel, Storage, Redundancy, Durability, DataProtection]
---

## 📚 Idea/Concept

Locally Redundant Storage (LRS) is the most basic and cost-effective data redundancy option in cloud storage, maintaining multiple synchronous copies of data within a single physical data center location, protecting against hardware failures but not against facility-level disasters.

### Concept Overview

LRS Architecture:
```
┌─────────────────────────────────────────────────────────┐
│                  Single Data Center                      │
│                                                          │
│  ┌─────────────────────────────────────────────────┐    │
│  │                   Storage Cluster                │    │
│  │                                                  │    │
│  │  Rack A          Rack B          Rack C         │    │
│  │  ┌─────┐         ┌─────┐         ┌─────┐       │    │
│  │  │Copy │         │Copy │         │Copy │       │    │
│  │  │  1  │         │  2  │         │  3  │       │    │
│  │  └─────┘         └─────┘         └─────┘       │    │
│  │     │               │               │          │    │
│  │     └───────────────┼───────────────┘          │    │
│  │                     │                           │    │
│  │              Synchronous                        │    │
│  │              Replication                        │    │
│  └─────────────────────────────────────────────────┘    │
│                                                          │
│  ✓ Protected: Disk failure, server failure              │
│  ✗ Not Protected: Data center failure, regional disaster│
└─────────────────────────────────────────────────────────┘
```

### Replication Mechanism

Synchronous Write Process:
```
Client Write Request
        │
        ▼
┌───────────────┐
│ Storage Front │
│     End       │
└───────┬───────┘
        │
        ├──────────┬──────────┐
        ▼          ▼          ▼
    ┌──────┐   ┌──────┐   ┌──────┐
    │Copy 1│   │Copy 2│   │Copy 3│
    └──────┘   └──────┘   └──────┘
        │          │          │
        └──────────┼──────────┘
                   │
              All copies
              confirmed
                   │
                   ▼
         Acknowledge to Client
```

Characteristics:
- Minimum 3 copies within data center
- Synchronous replication (write confirmed after all copies)
- Copies spread across different fault domains (racks, servers)
- 11 nines (99.999999999%) durability within data center

### Fault Domain Distribution

Within Data Center:
```
┌─────────────────────────────────────────────────────────┐
│                    Data Center                          │
│                                                         │
│  Fault Domain 1    Fault Domain 2    Fault Domain 3    │
│  (Rack/Server)     (Rack/Server)     (Rack/Server)     │
│                                                         │
│  ┌─────────────┐   ┌─────────────┐   ┌─────────────┐  │
│  │   Server    │   │   Server    │   │   Server    │  │
│  │  ┌───────┐  │   │  ┌───────┐  │   │  ┌───────┐  │  │
│  │  │ Copy  │  │   │  │ Copy  │  │   │  │ Copy  │  │  │
│  │  │   1   │  │   │  │   2   │  │   │  │   3   │  │  │
│  │  └───────┘  │   │  └───────┘  │   │  └───────┘  │  │
│  └─────────────┘   └─────────────┘   └─────────────┘  │
│                                                         │
│  Separate: Power, Network, Cooling                     │
└─────────────────────────────────────────────────────────┘
```

Protection provided:
| Failure Type | Protected | Recovery |
|--------------|-----------|----------|
| Disk failure | ✓ | Automatic |
| Server failure | ✓ | Automatic |
| Rack failure | ✓ | Automatic |
| Data center failure | ✗ | Data loss |
| Regional disaster | ✗ | Data loss |

### Durability vs Availability

Understanding the metrics:
```
Durability (99.999999999% - 11 nines):
- Probability of NOT losing data over a year
- LRS: 11 nines within data center
- Meaning: Extremely unlikely to lose stored data

Availability (99.9% - 3 nines):
- Probability of being able to access data
- LRS: Dependent on single data center
- Meaning: Some downtime expected annually

Annual Impact:
Durability 11 nines: 0.000000001% chance of data loss
Availability 99.9%: ~8.76 hours potential downtime/year
```

### Use Cases

Appropriate for LRS:
| Scenario | Rationale |
|----------|-----------|
| Development/Test | Non-critical, cost-sensitive |
| Temporary data | Short-lived, easily recreated |
| Easily reproducible data | Can be regenerated from source |
| Secondary/backup copies | Primary exists elsewhere |
| Cost-sensitive workloads | Budget constraints |

Not appropriate for LRS:
| Scenario | Better Option |
|----------|---------------|
| Production databases | ZRS or GRS |
| Business-critical data | ZRS minimum |
| Compliance requirements | GRS with GZRS |
| Disaster recovery source | GRS |

### Cost Comparison

Relative pricing (LRS as baseline):
```
Storage Redundancy Cost Comparison:

LRS  ████████████ 1.0x (baseline)
ZRS  ████████████████ 1.25x
GRS  ████████████████████ 1.5x
GZRS ████████████████████████ 2.0x

Note: Actual prices vary by provider and region
```

### Provider Implementations

| Provider | LRS Equivalent | Notes |
|----------|----------------|-------|
| Azure | LRS | Explicit option |
| AWS | Standard (single AZ) | S3 One Zone-IA |
| GCP | Regional | Single region storage |

### Data Recovery Considerations

When LRS data is lost:
```
Recovery Options:

1. Backup Restoration
   └── Restore from separate backup location
   
2. Rebuild from Source
   └── Regenerate from application/upstream
   
3. Accept Loss
   └── For truly ephemeral data

Prevention Strategies:
- Use LRS + external backups to different region
- Implement application-level redundancy
- Regular backup verification
```

### Decision Matrix

When to choose LRS:
```
                          Choose LRS?
                               │
                               ▼
                    ┌──────────────────┐
                    │ Is data easily   │
                    │ reproducible?    │
                    └────────┬─────────┘
                         Yes │ No
                    ┌────────┴────────┐
                    ▼                 ▼
              ┌──────────┐     ┌──────────────┐
              │   LRS    │     │ Compliance   │
              │   OK     │     │ requirements?│
              └──────────┘     └──────┬───────┘
                                  Yes │ No
                              ┌───────┴───────┐
                              ▼               ▼
                        ┌─────────┐    ┌──────────────┐
                        │Use GRS/ │    │ Data center  │
                        │  GZRS   │    │ failure      │
                        └─────────┘    │ acceptable?  │
                                       └──────┬───────┘
                                          Yes │ No
                                      ┌───────┴───────┐
                                      ▼               ▼
                                ┌─────────┐    ┌─────────┐
                                │   LRS   │    │   ZRS   │
                                │   OK    │    │ minimum │
                                └─────────┘    └─────────┘
```

## 📌 Key Points (Optional)
- LRS provides multiple copies within a single data center for hardware failure protection.
- Does not protect against data center or regional disasters.
- Most cost-effective option, suitable for non-critical or reproducible data.
- 11 nines durability applies only within the data center boundary.

## 🖼️ Recommended Image
- Diagram showing data copies spread across different racks within single data center.

## 🔗 Connections
- [[188-Zone Redundant Storage ZRS]]
- [[189-Geo Redundant Storage GRS]]
- [[185-Virtual Private Cloud VPC]]