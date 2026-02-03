---
Theme: Non Functional Requirements
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, Requirements, Quality, Architecture]
---

## 📚 Idea/Concept

Non-Functional Requirements (NFRs) specify quality criteria and constraints on how the system must operate, rather than what functions it must perform. They define system attributes such as performance, security, usability, reliability, availability, maintainability, and scalability. These requirements are critical for user satisfaction and system success, though often less visible than functional requirements. They significantly impact system architecture and technical decisions. Also known as "quality attributes" or "quality requirements."

### FURPS+ Quality Model

```
┌─────────────────────────────────────────────────────────────────┐
│                        FURPS+ MODEL                             │
├─────────────────────────────────────────────────────────────────┤
│  F - FUNCTIONALITY                                              │
│      (Covered by Functional Requirements)                       │
├─────────────────────────────────────────────────────────────────┤
│  U - USABILITY                                                  │
│      • Learnability        • Accessibility                      │
│      • Efficiency          • User satisfaction                  │
│      • Memorability        • Error prevention                   │
├─────────────────────────────────────────────────────────────────┤
│  R - RELIABILITY                                                │
│      • Availability        • Fault tolerance                    │
│      • Recoverability      • Data integrity                     │
│      • Mean Time Between Failures (MTBF)                        │
├─────────────────────────────────────────────────────────────────┤
│  P - PERFORMANCE                                                │
│      • Response time       • Throughput                         │
│      • Resource utilization• Scalability                        │
│      • Latency             • Capacity                           │
├─────────────────────────────────────────────────────────────────┤
│  S - SUPPORTABILITY                                             │
│      • Maintainability     • Testability                        │
│      • Configurability     • Installability                     │
│      • Localizability      • Portability                        │
├─────────────────────────────────────────────────────────────────┤
│  + ADDITIONAL CONSTRAINTS                                       │
│      • Design constraints  • Implementation constraints         │
│      • Interface constraints• Physical constraints              │
└─────────────────────────────────────────────────────────────────┘
```

### NFR Examples with Metrics

```
┌─────────────────────────────────────────────────────────────────┐
│                    NFR SPECIFICATION FORMAT                     │
├─────────────────────────────────────────────────────────────────┤
│  IDENTIFIER:    NFR-001                                         │
│  CATEGORY:      Performance                                     │
│  ATTRIBUTE:     Response Time                                   │
├─────────────────────────────────────────────────────────────────┤
│  REQUIREMENT:                                                   │
│  The system shall respond to user requests within 2 seconds     │
│  for 95% of all transactions under normal load conditions.      │
├─────────────────────────────────────────────────────────────────┤
│  MEASUREMENT:                                                   │
│  • Metric: Server response time                                 │
│  • Method: Application Performance Monitoring (APM)             │
│  • Target: < 2 seconds (95th percentile)                       │
│  • Threshold: < 5 seconds (99th percentile)                    │
├─────────────────────────────────────────────────────────────────┤
│  VERIFICATION:                                                  │
│  • Load testing with 1000 concurrent users                      │
│  • Production monitoring dashboards                             │
└─────────────────────────────────────────────────────────────────┘
```

### Common NFR Categories

| Category | Metric Examples | Tools |
|----------|-----------------|-------|
| Performance | Response time < 2s, 1000 TPS | JMeter, Gatling |
| Availability | 99.9% uptime (8.76h/year downtime) | Monitoring, SLAs |
| Security | OWASP compliance, encryption | OWASP ZAP, Burp |
| Scalability | Handle 10x traffic spike | Load testing |
| Usability | Task completion < 3 clicks | User testing |
| Maintainability | Code coverage > 80% | SonarQube |

### NFR Trade-offs

```
                    TRADE-OFF TENSIONS
                    
    Security ←──────────────────────→ Usability
    (More checks = more friction)
    
    Performance ←───────────────────→ Cost
    (Better hardware = more expensive)
    
    Availability ←──────────────────→ Consistency
    (CAP theorem in distributed systems)
    
    Flexibility ←───────────────────→ Simplicity
    (More options = more complexity)
```

### Impact on Architecture

```
NFR: "System must handle 10,000 concurrent users"

Architecture Implications:
├── Load balancing required
├── Horizontal scaling capability
├── Database connection pooling
├── Caching layer (Redis/Memcached)
├── CDN for static assets
└── Async processing for heavy operations

Cost: 60-80% of development effort addresses NFRs
```

### NFR in Agile

```
How to handle NFRs in Scrum:

1. As Acceptance Criteria
   User Story: "As a user, I want to search products"
   NFR Criteria: "Results display within 1 second"

2. As Technical Stories
   "Implement caching layer for product search"

3. As Definition of Done
   "All features must pass security scan"

4. As Constraints
   "All APIs must use HTTPS"
```

## 📌 Key Points (Optional)
- NFRs are the silent heroes of development
- A system can do everything functionally correct, but if it's slow, insecure, or hard to use, it will fail
- The most costly mistake is treating them as "nice to have" or leaving them for the end
- NFRs must be defined early because retrofitting is extremely expensive
- My rule: for every User Story, ask "How fast? How secure? How easy?"
- Poorly defined NFRs are the main cause of technical debt

## 🖼️ Recommended Image
- Quality attributes spider/radar diagram showing NFR dimensions

## 🔗 Connections
- [[215-Functional Requirements]]
- [[230-Definition of Done]]
- [[220-Product Backlog]]
- [[218-User Story]]
- [[219-Software Requirements Specification]]