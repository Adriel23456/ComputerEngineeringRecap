---
Theme: Kubernetes Container Orchestration
Date created: 2026-02-02 12:00
tags: [CloudLevel, Kubernetes, Containers, Orchestration, CloudNative]
---

## 📚 Idea/Concept

Kubernetes (K8s) is an open-source container orchestration platform that automates the deployment, scaling, and management of containerized applications. Originally developed by Google and now maintained by CNCF, it has become the de facto standard for running containers at scale in production.

### Concept Overview

What Kubernetes Provides:
```
┌─────────────────────────────────────────────────────────────────┐
│              Kubernetes Orchestration Capabilities              │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐ │
│  │ Service         │  │ Self-Healing    │  │ Auto-Scaling    │ │
│  │ Discovery &     │  │ (Restart failed │  │ (Scale based    │ │
│  │ Load Balancing  │  │  containers)    │  │  on metrics)    │ │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘ │
│                                                                  │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐ │
│  │ Rolling         │  │ Configuration   │  │ Storage         │ │
│  │ Updates &       │  │ & Secrets       │  │ Orchestration   │ │
│  │ Rollbacks       │  │ Management      │  │                 │ │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘ │
│                                                                  │
│  Declarative: Describe WHAT you want, K8s figures out HOW      │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Cluster Architecture

Control Plane and Worker Nodes:
```
┌─────────────────────────────────────────────────────────────────────────┐
│                        KUBERNETES CLUSTER                                │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                          │
│  ┌────────────────────────────────────────────────────────────────┐    │
│  │                     CONTROL PLANE (Master)                      │    │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────────────┐  │    │
│  │  │ API Server   │  │  Scheduler   │  │ Controller Manager   │  │    │
│  │  │ (kube-api)   │  │              │  │ - Node Controller    │  │    │
│  │  │              │  │ Assigns pods │  │ - Replication Ctrl   │  │    │
│  │  │ Entry point  │  │ to nodes     │  │ - Endpoint Ctrl      │  │    │
│  │  └──────────────┘  └──────────────┘  └──────────────────────┘  │    │
│  │  ┌──────────────┐  ┌──────────────────────────────────────────┐│    │
│  │  │    etcd      │  │        Cloud Controller Manager          ││    │
│  │  │ (Key-Value   │  │   (Provider-specific: AWS, Azure, GCP)  ││    │
│  │  │  Store)      │  └──────────────────────────────────────────┘│    │
│  │  └──────────────┘                                               │    │
│  └────────────────────────────────────────────────────────────────┘    │
│                                │                                        │
│                                │ API calls                              │
│                                ▼                                        │
│  ┌────────────────────────────────────────────────────────────────┐    │
│  │                      WORKER NODES                               │    │
│  │                                                                  │    │
│  │  ┌─────────────────────────────┐  ┌─────────────────────────┐  │    │
│  │  │         Node 1              │  │         Node 2          │  │    │
│  │  │  ┌───────────────────────┐  │  │  ┌───────────────────┐  │  │    │
│  │  │  │       kubelet         │  │  │  │     kubelet       │  │  │    │
│  │  │  │  (Node agent)         │  │  │  │                   │  │  │    │
│  │  │  └───────────────────────┘  │  │  └───────────────────┘  │  │    │
│  │  │  ┌───────────────────────┐  │  │  ┌───────────────────┐  │  │    │
│  │  │  │      kube-proxy       │  │  │  │    kube-proxy     │  │  │    │
│  │  │  │  (Network proxy)      │  │  │  │                   │  │  │    │
│  │  │  └───────────────────────┘  │  │  └───────────────────┘  │  │    │
│  │  │  ┌───────────────────────┐  │  │  ┌───────────────────┐  │  │    │
│  │  │  │  Container Runtime    │  │  │  │ Container Runtime │  │  │    │
│  │  │  │  (containerd/CRI-O)   │  │  │  │                   │  │  │    │
│  │  │  └───────────────────────┘  │  │  └───────────────────┘  │  │    │
│  │  │  ┌─────┐┌─────┐┌─────┐     │  │  ┌─────┐┌─────┐        │  │    │
│  │  │  │ Pod ││ Pod ││ Pod │     │  │  │ Pod ││ Pod │        │  │    │
│  │  │  └─────┘└─────┘└─────┘     │  │  └─────┘└─────┘        │  │    │
│  │  └─────────────────────────────┘  └─────────────────────────┘  │    │
│  └────────────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────────┘
```

### Core Objects

Pod - Smallest deployable unit:
```yaml
# pod.yaml
apiVersion: v1
kind: Pod
metadata:
  name: my-app
  labels:
    app: my-app
spec:
  containers:
  - name: app
    image: myapp:1.0
    ports:
    - containerPort: 8080
    resources:
      requests:
        memory: "128Mi"
        cpu: "250m"
      limits:
        memory: "256Mi"
        cpu: "500m"
    livenessProbe:
      httpGet:
        path: /health
        port: 8080
      initialDelaySeconds: 10
      periodSeconds: 5
```

Pod structure:
```
┌───────────────────────────────────────────────────────┐
│                         POD                           │
│                   (Shared context)                    │
│                                                       │
│  ┌─────────────────┐  ┌─────────────────┐           │
│  │   Container 1   │  │   Container 2   │           │
│  │   (Main App)    │  │   (Sidecar)     │           │
│  └─────────────────┘  └─────────────────┘           │
│                                                       │
│  Shared:                                              │
│  - Network namespace (same IP, localhost)            │
│  - Storage volumes (if mounted)                      │
│  - IPC namespace                                     │
│                                                       │
│  Pod IP: 10.244.1.15                                 │
└───────────────────────────────────────────────────────┘
```

### Workload Resources

Deployment - Declarative updates:
```yaml
# deployment.yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: my-app
spec:
  replicas: 3
  selector:
    matchLabels:
      app: my-app
  strategy:
    type: RollingUpdate
    rollingUpdate:
      maxSurge: 1
      maxUnavailable: 0
  template:
    metadata:
      labels:
        app: my-app
    spec:
      containers:
      - name: app
        image: myapp:1.0
        ports:
        - containerPort: 8080
```

Deployment creates ReplicaSet which manages Pods:
```
┌─────────────────────────────────────────────────────────────────┐
│                                                                  │
│  Deployment (my-app)                                            │
│  ├── ReplicaSet (my-app-abc123) - current                      │
│  │   ├── Pod (my-app-abc123-xyz1)                              │
│  │   ├── Pod (my-app-abc123-xyz2)                              │
│  │   └── Pod (my-app-abc123-xyz3)                              │
│  │                                                              │
│  └── ReplicaSet (my-app-def456) - previous (scaled to 0)       │
│                                                                  │
│  Rolling Update Process:                                        │
│  replicas: 3 → 3, maxSurge: 1, maxUnavailable: 0               │
│                                                                  │
│  Step 1: [v1][v1][v1][v2]  (surge to 4)                        │
│  Step 2: [v1][v1][v2][v2]  (replace 1)                         │
│  Step 3: [v1][v2][v2][v2]  (replace 2)                         │
│  Step 4: [v2][v2][v2]      (complete, scale down)              │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Service - Network Abstraction

Exposing applications:
```yaml
# service.yaml
apiVersion: v1
kind: Service
metadata:
  name: my-app-service
spec:
  type: ClusterIP  # or NodePort, LoadBalancer
  selector:
    app: my-app    # matches pod labels
  ports:
  - port: 80       # service port
    targetPort: 8080  # container port
```

Service types:
```
┌─────────────────────────────────────────────────────────────────┐
│                      Service Types                               │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  ClusterIP (default):                                           │
│  - Internal cluster IP only                                     │
│  - Accessible only within cluster                               │
│  ┌─────────────┐                                               │
│  │ ClusterIP   │ ──► Pod1, Pod2, Pod3                          │
│  │ 10.96.0.100 │                                               │
│  └─────────────┘                                               │
│                                                                  │
│  NodePort:                                                      │
│  - Exposes on each node's IP at static port                    │
│  - Range: 30000-32767                                          │
│  ┌─────────────┐                                               │
│  │External:    │                                               │
│  │NodeIP:30080 │ ──► ClusterIP ──► Pods                        │
│  └─────────────┘                                               │
│                                                                  │
│  LoadBalancer:                                                  │
│  - Provisions cloud load balancer                              │
│  - External IP assigned                                        │
│  ┌─────────────┐                                               │
│  │ Cloud LB    │                                               │
│  │52.10.20.30  │ ──► NodePort ──► ClusterIP ──► Pods           │
│  └─────────────┘                                               │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Ingress - HTTP Routing

Layer 7 load balancing:
```yaml
# ingress.yaml
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: my-ingress
  annotations:
    nginx.ingress.kubernetes.io/rewrite-target: /
spec:
  ingressClassName: nginx
  tls:
  - hosts:
    - myapp.example.com
    secretName: tls-secret
  rules:
  - host: myapp.example.com
    http:
      paths:
      - path: /api
        pathType: Prefix
        backend:
          service:
            name: api-service
            port:
              number: 80
      - path: /web
        pathType: Prefix
        backend:
          service:
            name: web-service
            port:
              number: 80
```

Ingress routing:
```
                    Internet
                        │
                        ▼
              ┌─────────────────┐
              │ Ingress Controller│
              │  (nginx, traefik) │
              └────────┬──────────┘
                       │
        ┌──────────────┼──────────────┐
        │              │              │
  /api/*          /web/*        /admin/*
        │              │              │
        ▼              ▼              ▼
  ┌──────────┐  ┌──────────┐  ┌──────────┐
  │api-service│  │web-service│  │admin-svc │
  └──────────┘  └──────────┘  └──────────┘
```

### Configuration Management

ConfigMaps and Secrets:
```yaml
# configmap.yaml
apiVersion: v1
kind: ConfigMap
metadata:
  name: app-config
data:
  DATABASE_HOST: "postgres.default.svc"
  LOG_LEVEL: "info"
  config.json: |
    {
      "feature_flags": {
        "new_ui": true
      }
    }

---
# secret.yaml
apiVersion: v1
kind: Secret
metadata:
  name: app-secrets
type: Opaque
data:
  DATABASE_PASSWORD: cGFzc3dvcmQxMjM=  # base64 encoded
  API_KEY: c2VjcmV0a2V5MTIz

---
# Using in deployment
spec:
  containers:
  - name: app
    envFrom:
    - configMapRef:
        name: app-config
    - secretRef:
        name: app-secrets
    volumeMounts:
    - name: config-volume
      mountPath: /etc/config
  volumes:
  - name: config-volume
    configMap:
      name: app-config
```

### Horizontal Pod Autoscaler

Automatic scaling:
```yaml
# hpa.yaml
apiVersion: autoscaling/v2
kind: HorizontalPodAutoscaler
metadata:
  name: my-app-hpa
spec:
  scaleTargetRef:
    apiVersion: apps/v1
    kind: Deployment
    name: my-app
  minReplicas: 2
  maxReplicas: 10
  metrics:
  - type: Resource
    resource:
      name: cpu
      target:
        type: Utilization
        averageUtilization: 70
  - type: Resource
    resource:
      name: memory
      target:
        type: Utilization
        averageUtilization: 80
```

Scaling behavior:
```
                CPU Utilization
                     │
  Scale Up ──────────┼──────────── 70% target
                     │
                     │   ┌─────┐
        Pods: 2      │  ╱       ╲
                     │ ╱  Load   ╲
                     │╱   Spike   ╲
  ──────────────────────────────────────► Time
                     │
        Pods: 2 → 4 → 6 → 4 → 2
                     │
  Scale Down ────────┼────────────
                     │
```

### Managed Kubernetes Services

Provider offerings:
```
┌─────────────────────────────────────────────────────────────────┐
│                  Managed Kubernetes Services                     │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  AWS EKS (Elastic Kubernetes Service)                           │
│  - Managed control plane                                        │
│  - Deep AWS integration (IAM, VPC, ALB)                        │
│  - Fargate option for serverless pods                          │
│                                                                  │
│  Azure AKS (Azure Kubernetes Service)                           │
│  - Free control plane                                           │
│  - Azure AD integration                                         │
│  - Virtual nodes (ACI)                                          │
│                                                                  │
│  GCP GKE (Google Kubernetes Engine)                             │
│  - Most mature (Google created K8s)                            │
│  - Autopilot mode (fully managed nodes)                        │
│  - Best default security                                        │
│                                                                  │
│  All provide:                                                   │
│  - Managed control plane (HA, upgrades)                        │
│  - Node pool management                                         │
│  - Integrated networking and storage                           │
│  - Logging and monitoring                                       │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

## 📌 Key Points (Optional)
- Kubernetes provides declarative infrastructure for container workloads.
- Pods are the smallest unit; Deployments manage replicas and updates.
- Services provide stable networking; Ingress handles HTTP routing.
- HPA enables automatic scaling based on metrics.

## 🖼️ Recommended Image
- Kubernetes architecture diagram showing control plane, worker nodes, and core objects relationship.

## 🔗 Connections
- [[200-Containerization Fundamentals]]
- [[190-Horizontal and Vertical Scaling in Cloud]]
- [[192-Hybrid Cloud Architecture]]
- [[184-Major Cloud Providers Comparison]]
- [[193-Application Availability]]
- [[203-Microservices Architecture]]