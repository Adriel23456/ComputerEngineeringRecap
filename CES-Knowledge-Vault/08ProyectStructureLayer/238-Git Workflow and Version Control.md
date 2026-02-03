---
Theme: Git Workflow and Version Control
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, Git, VersionControl, Collaboration]
---

## 📚 Idea/Concept

Git is a distributed version control system that tracks changes in source code during software development. A Git Workflow defines how teams use Git to collaborate effectively, manage code changes, and maintain a clean project history. Understanding branching strategies, commits, pull requests, and team roles is essential for professional software development and team collaboration.

### Git Core Concepts

```
┌─────────────────────────────────────────────────────────────────┐
│                     GIT FUNDAMENTALS                            │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  REPOSITORY (Repo)                                              │
│  └── Container for project files and version history           │
│                                                                 │
│  COMMIT                                                         │
│  └── Snapshot of changes with message and unique ID (SHA)      │
│                                                                 │
│  BRANCH                                                         │
│  └── Independent line of development                           │
│                                                                 │
│  MERGE                                                          │
│  └── Combining changes from different branches                 │
│                                                                 │
│  PULL REQUEST (PR) / MERGE REQUEST (MR)                        │
│  └── Request to merge changes with code review                 │
│                                                                 │
│  REMOTE                                                         │
│  └── Server-hosted repository (GitHub, GitLab, etc.)          │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Branch Structure

```
┌─────────────────────────────────────────────────────────────────┐
│                    BRANCH HIERARCHY                             │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  master/main ─────────────────────────────────────────────────  │
│       │                                                         │
│       │ (always stable, production-ready)                      │
│       │                                                         │
│       ├─── develop ───────────────────────────────────────────  │
│       │        │                                                │
│       │        │ (integration branch for features)             │
│       │        │                                                │
│       │        ├─── feature/user-auth ────────────────────     │
│       │        │         │                                      │
│       │        │         │ (new functionality)                 │
│       │        │         │                                      │
│       │        ├─── feature/shopping-cart ────────────────     │
│       │        │                                                │
│       │        └─── feature/payment-gateway ──────────────     │
│       │                                                         │
│       ├─── release/v1.0 ──────────────────────────────────     │
│       │        │                                                │
│       │        │ (preparation for release)                     │
│       │                                                         │
│       └─── hotfix/security-patch ─────────────────────────     │
│                │                                                │
│                │ (urgent production fixes)                     │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Feature Branch Workflow

```
┌─────────────────────────────────────────────────────────────────┐
│                 FEATURE BRANCH WORKFLOW                         │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  1. CREATE FEATURE BRANCH                                      │
│     git checkout develop                                       │
│     git pull origin develop                                    │
│     git checkout -b feature/user-authentication                │
│                                                                 │
│  2. DEVELOP AND COMMIT                                         │
│     # Make changes                                             │
│     git add .                                                  │
│     git commit -m "feat: add login form validation"            │
│     # Continue development...                                  │
│     git commit -m "feat: implement JWT authentication"         │
│                                                                 │
│  3. PUSH TO REMOTE                                             │
│     git push origin feature/user-authentication                │
│                                                                 │
│  4. CREATE PULL REQUEST                                        │
│     • Open PR from feature → develop                          │
│     • Add description and reviewers                           │
│     • Link to related issues/tickets                          │
│                                                                 │
│  5. CODE REVIEW                                                │
│     • Reviewers examine changes                               │
│     • Request changes if needed                               │
│     • Approve when ready                                      │
│                                                                 │
│  6. MERGE AND CLEANUP                                          │
│     • Merge PR (squash or merge commit)                       │
│     • Delete feature branch                                    │
│     git branch -d feature/user-authentication                  │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Commit Message Convention

```
┌─────────────────────────────────────────────────────────────────┐
│              CONVENTIONAL COMMITS FORMAT                        │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  FORMAT:                                                        │
│  <type>(<scope>): <description>                                │
│                                                                 │
│  [optional body]                                               │
│                                                                 │
│  [optional footer]                                             │
│                                                                 │
│  ───────────────────────────────────────────────────────────── │
│                                                                 │
│  TYPES:                                                        │
│  feat:     New feature                                         │
│  fix:      Bug fix                                             │
│  docs:     Documentation only                                  │
│  style:    Formatting, no code change                          │
│  refactor: Code change without feat/fix                        │
│  test:     Adding tests                                        │
│  chore:    Maintenance tasks                                   │
│                                                                 │
│  ───────────────────────────────────────────────────────────── │
│                                                                 │
│  EXAMPLES:                                                      │
│                                                                 │
│  feat(auth): add password reset functionality                  │
│                                                                 │
│  fix(cart): resolve quantity update bug                        │
│                                                                 │
│  docs(readme): update installation instructions                │
│                                                                 │
│  refactor(api): extract validation to middleware               │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Pull Request Best Practices

```
┌─────────────────────────────────────────────────────────────────┐
│                  PULL REQUEST TEMPLATE                          │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ## Description                                                 │
│  Brief description of changes and why they were made           │
│                                                                 │
│  ## Type of Change                                             │
│  - [ ] Bug fix                                                 │
│  - [ ] New feature                                             │
│  - [ ] Breaking change                                         │
│  - [ ] Documentation update                                    │
│                                                                 │
│  ## Related Issues                                             │
│  Closes #123                                                   │
│                                                                 │
│  ## Testing                                                    │
│  - [ ] Unit tests added/updated                                │
│  - [ ] Manual testing completed                                │
│                                                                 │
│  ## Checklist                                                  │
│  - [ ] Code follows style guidelines                           │
│  - [ ] Self-review completed                                   │
│  - [ ] Documentation updated                                   │
│  - [ ] No new warnings                                         │
│                                                                 │
│  ## Screenshots (if applicable)                                │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Git Team Roles

```
┌─────────────────────────────────────────────────────────────────┐
│                     GIT TEAM ROLES                              │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  MAINTAINER / OWNER                                            │
│  • Full repository access                                      │
│  • Merge to master/main                                        │
│  • Manage repository settings                                  │
│  • Configure branch protections                                │
│                                                                 │
│  COLLABORATOR / DEVELOPER                                      │
│  • Create feature branches                                     │
│  • Submit pull requests                                        │
│  • Review others' code                                         │
│  • Merge to develop (with approval)                           │
│                                                                 │
│  REVIEWER                                                       │
│  • Review pull requests                                        │
│  • Approve/request changes                                     │
│  • Enforce code standards                                      │
│                                                                 │
│  CONTRIBUTOR (External)                                        │
│  • Fork repository                                             │
│  • Submit PRs from fork                                        │
│  • Cannot directly push                                        │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Branch Protection Rules

```
RECOMMENDED PROTECTIONS FOR MAIN BRANCHES:

master/main:
├── Require pull request reviews (minimum 2)
├── Require status checks to pass
├── Require branches to be up to date
├── Include administrators in restrictions
├── Restrict who can push (maintainers only)
└── Require signed commits

develop:
├── Require pull request reviews (minimum 1)
├── Require status checks to pass
└── Allow squash merging
```

## 📌 Key Points (Optional)
- Master should always be stable and deployable
- Feature branches keep development isolated
- Small, focused commits make history readable
- Pull requests enable code review and knowledge sharing
- Consistent commit messages improve project maintainability
- Branch protection prevents accidental mistakes

## 🖼️ Recommended Image
- Git flow diagram showing branch relationships and merge directions

## 🔗 Connections
- [[237-Coding Standards]]
- [[230-Definition of Done]]
- [[212-Development Team]]
- [[239-Azure DevOps]]
- [[240-Jira Agile Tool]]