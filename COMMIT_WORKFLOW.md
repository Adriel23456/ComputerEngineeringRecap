# COMMIT_WORKFLOW.md

## Purpose

This document defines the **official commit and branching workflow** for the project.

Its goal is to:
- Enforce **clean history**
- Enable **safe parallel development**
- Support **iterative design without polluting `master`**
- Maintain a **professional, auditable Git timeline**

This workflow is **mandatory** for all contributors and future development.

---

## Core Principles

1. **`master` is always stable**
   - No experimental or partial work is committed directly to `master`
   - Every commit on `master` represents a coherent, buildable state

2. **All development is feature-driven**
   - Every meaningful change lives inside a `feature/*` branch
   - Features are merged only when logically complete

3. **Integration happens through a dedicated development branch**
   - `developApp2` is the **integration and validation branch**
   - This is where features converge before release

4. **Tags represent milestones, not experiments**
   - Tags are applied only on `master`
   - Each tag represents a meaningful architectural or functional milestone

---

## Branch Structure Overview

```

master
│
├── developApp2
│     ├── feature/Initial_Design
│     ├── feature/<feature-name>
│     └── feature/<another-feature>
│
└── (tags)

```

---

## Branch Roles

### `master`

**Role:**  
Stable, production-grade history

**Rules:**
- No direct commits
- Only fast-forward or merge commits from `developApp2`
- Every commit must compile and run
- Tagged releases are created here

**What belongs here:**
- Completed architectural steps
- Stable program milestones
- Release-ready states

---

### `developApp2`

**Role:**  
Main integration and staging branch

**Rules:**
- Receives merges from `feature/*` branches
- Can temporarily contain in-progress integrations
- Must remain reasonably stable, but not release-perfect

**What belongs here:**
- Fully implemented steps
- Integrated features awaiting final polish
- Cross-feature coordination work

---

### `feature/*` branches

**Role:**  
Isolated development of a single concern

**Naming Convention:**
```

feature/<descriptive_name>

```

Examples:
- `feature/Initial_Design`
- `feature/Quicksort_Grid_Rendering`
- `feature/OpenMP_Logic_Thread`

**Rules:**
- Branch from `developApp2`
- Focus on **one responsibility only**
- Can contain multiple commits
- Rebased or squashed before merge if needed

**What belongs here:**
- Experimental code
- Step-by-step implementation
- Partial logic and scaffolding
- Refactors related to the feature

---

## Commit Granularity Rules

### Inside `feature/*` branches
Commits may be:
- Incremental
- Exploratory
- Technical scaffolding
- Refactors
- Debug commits

Example:
```

feat: scaffold quicksort visualizer state
refactor: extract grid camera controller
debug: verify OpenMP thread initialization

```

---

### Merging into `developApp2`

Before merging:
- Feature must compile
- Feature must align with architecture rules
- Feature scope must be complete

Preferred merge strategy:
- **Squash merge** if commits are noisy
- **Normal merge** if commit history is clean and meaningful

---

### Merging into `master`

**Only from `developApp2`**

Conditions:
- A logical milestone is complete
- Code is stable
- Architecture is preserved
- Documentation is updated if needed

---

## Tagging Strategy

Tags are **mandatory** for milestones.

### Tag Format

```

v<major>.<minor>.<patch>

```

Examples:
- `v1.0.0` – First complete architectural baseline
- `v1.1.0` – New program added (Quicksort Visualizer)
- `v1.2.0` – Major visualization system upgrade

### When to Tag
- After merging `developApp2` into `master`
- When a step or phase is **fully complete**
- Never tag intermediate or experimental states

---

## Example Workflow (End-to-End)

1. Create feature branch:
```

git checkout developApp2
git checkout -b feature/Initial_Design

```

2. Commit freely during development:
```

feat: create quicksort visualizer state skeleton
feat: integrate program into main menu
refactor: align state lifecycle with engine

```

3. Merge into `developApp2`:
```

git checkout developApp2
git merge feature/Initial_Design

```

4. Validate integration

5. Merge into `master`:
```

git checkout master
git merge developApp2

```

6. Tag the milestone:
```

git tag v1.1.0

```

---

## Non-Negotiable Rules

- ❌ No direct commits to `master`
- ❌ No long-lived feature branches
- ❌ No mixing unrelated concerns in a single feature
- ❌ No tags outside `master`

- ✅ One feature = one responsibility
- ✅ Commits tell a clear story
- ✅ Architecture > speed
- ✅ Stability before release

---

## Final Note

This workflow is designed to support:
- **Large-scale systems**
- **Long-lived projects**
- **Educational and professional-grade codebases**

Deviation from this workflow must be **explicitly justified** and documented.

**This file defines the single source of truth for Git workflow in this project.**