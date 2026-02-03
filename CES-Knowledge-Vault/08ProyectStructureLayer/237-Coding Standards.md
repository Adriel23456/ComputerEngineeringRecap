---
Theme: Coding Standards
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, BestPractices, Quality, Development]
---

## 📚 Idea/Concept

Coding Standards are a set of rules, conventions, and best practices agreed upon by a development team to write consistent, maintainable, and high-quality code. They include naming conventions, code structure, formatting, documentation, error handling, and design patterns. These standards ensure that code is readable and maintainable by any team member, reduce technical debt, facilitate onboarding of new developers, and minimize bugs. They are fundamental for effective collaboration and long-term sustainable software quality.

### Areas of Coding Standards

```
┌─────────────────────────────────────────────────────────────────┐
│                   CODING STANDARDS AREAS                        │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  1. NAMING CONVENTIONS                                         │
│     • Variables, functions, classes                            │
│     • Files and folders                                        │
│     • Constants and enums                                      │
│                                                                 │
│  2. FORMATTING                                                  │
│     • Indentation (spaces vs tabs)                            │
│     • Line length                                              │
│     • Brace placement                                          │
│     • Whitespace usage                                         │
│                                                                 │
│  3. DOCUMENTATION                                               │
│     • File headers                                             │
│     • Function/method comments                                 │
│     • Inline comments                                          │
│     • README files                                             │
│                                                                 │
│  4. CODE ORGANIZATION                                          │
│     • File structure                                           │
│     • Module organization                                      │
│     • Import ordering                                          │
│     • Function length limits                                   │
│                                                                 │
│  5. ERROR HANDLING                                             │
│     • Exception handling patterns                              │
│     • Logging standards                                        │
│     • Error messages                                           │
│                                                                 │
│  6. TESTING                                                    │
│     • Test naming                                              │
│     • Test organization                                        │
│     • Coverage requirements                                    │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Naming Conventions

```
┌─────────────────────────────────────────────────────────────────┐
│                   NAMING CONVENTIONS                            │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  CASE STYLES:                                                   │
│                                                                 │
│  camelCase       │ Variables, functions (JS, Java)             │
│  PascalCase      │ Classes, types (C#, TypeScript)             │
│  snake_case      │ Variables, functions (Python, Ruby)         │
│  SCREAMING_CASE  │ Constants, enums                            │
│  kebab-case      │ URLs, file names, CSS classes               │
│                                                                 │
│  ───────────────────────────────────────────────────────────── │
│                                                                 │
│  NAMING RULES:                                                  │
│                                                                 │
│  ✓ Descriptive and meaningful                                  │
│    getUserById()  vs  get()                                    │
│    totalPrice     vs  tp                                       │
│                                                                 │
│  ✓ Verb for functions/methods                                  │
│    calculateTotal()  fetchUsers()  validateEmail()             │
│                                                                 │
│  ✓ Noun for classes/variables                                  │
│    User  Product  orderCount                                   │
│                                                                 │
│  ✓ Boolean prefix: is, has, can, should                        │
│    isActive  hasPermission  canEdit  shouldUpdate              │
│                                                                 │
│  ✓ Avoid abbreviations (except well-known)                     │
│    customerId  vs  custId                                      │
│    OK: URL, HTTP, ID, API                                      │
│                                                                 │
│  ✓ Consistent pluralization                                    │
│    users (collection)  user (single)                           │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Code Formatting Example

```
┌─────────────────────────────────────────────────────────────────┐
│                    FORMATTING EXAMPLE                           │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ❌ BAD:                                                        │
│  ────────────────────────────────────────────────────────────  │
│  function calculateTotal(items,tax,discount){                  │
│  let total=0;for(let i=0;i<items.length;i++){                  │
│  total+=items[i].price*items[i].quantity;}                     │
│  return total*(1+tax)*(1-discount);}                           │
│                                                                 │
│  ✓ GOOD:                                                       │
│  ────────────────────────────────────────────────────────────  │
│  function calculateTotal(items, tax, discount) {               │
│      let subtotal = 0;                                         │
│                                                                 │
│      for (const item of items) {                               │
│          subtotal += item.price * item.quantity;               │
│      }                                                         │
│                                                                 │
│      const taxAmount = subtotal * tax;                         │
│      const discountAmount = subtotal * discount;               │
│                                                                 │
│      return subtotal + taxAmount - discountAmount;             │
│  }                                                             │
│                                                                 │
│  IMPROVEMENTS:                                                  │
│  • Consistent spacing                                          │
│  • Meaningful variable names                                   │
│  • One operation per line                                      │
│  • Proper indentation                                          │
│  • Modern loop syntax                                          │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Clean Code Principles

```
┌─────────────────────────────────────────────────────────────────┐
│                  CLEAN CODE PRINCIPLES                          │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  DRY - Don't Repeat Yourself                                   │
│  ═══════════════════════════                                   │
│  • Extract common code into functions                          │
│  • Avoid copy-paste programming                                │
│  • Single source of truth                                      │
│                                                                 │
│  KISS - Keep It Simple, Stupid                                 │
│  ════════════════════════════                                  │
│  • Simplest solution that works                                │
│  • Avoid over-engineering                                      │
│  • Premature optimization is evil                              │
│                                                                 │
│  YAGNI - You Aren't Gonna Need It                              │
│  ════════════════════════════════                              │
│  • Don't build features "just in case"                        │
│  • Implement when needed, not before                           │
│                                                                 │
│  Single Responsibility                                          │
│  ════════════════════                                          │
│  • One function = one purpose                                  │
│  • One class = one reason to change                            │
│  • Keep functions under 20 lines                               │
│                                                                 │
│  Boy Scout Rule                                                 │
│  ═══════════════                                               │
│  • Leave code better than you found it                         │
│  • Small improvements add up                                   │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Documentation Standards

```
┌─────────────────────────────────────────────────────────────────┐
│              DOCUMENTATION STANDARDS                            │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  FUNCTION DOCUMENTATION (JSDoc style):                         │
│                                                                 │
│  /**                                                           │
│   * Calculates the total price including tax and discount.    │
│   *                                                            │
│   * @param {Array<Item>} items - Products to calculate         │
│   * @param {number} taxRate - Tax rate as decimal (e.g., 0.1) │
│   * @param {number} discount - Discount rate as decimal        │
│   * @returns {number} Total price after tax and discount       │
│   * @throws {Error} If items array is empty                    │
│   *                                                            │
│   * @example                                                   │
│   * calculateTotal([{price: 10, qty: 2}], 0.1, 0.05)          │
│   * // Returns: 20.90                                          │
│   */                                                           │
│  function calculateTotal(items, taxRate, discount) { }         │
│                                                                 │
│  ───────────────────────────────────────────────────────────── │
│                                                                 │
│  WHEN TO COMMENT:                                               │
│  ✓ Complex business logic                                      │
│  ✓ Non-obvious algorithms                                      │
│  ✓ Workarounds and known issues                               │
│  ✓ Public API functions                                        │
│                                                                 │
│  WHEN NOT TO COMMENT:                                           │
│  ✗ Obvious code (let count = 0; // set count to zero)         │
│  ✗ Poorly named code (fix the name instead)                    │
│  ✗ Commented-out code (delete it, use version control)        │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Enforcement Tools

| Language | Linter | Formatter | Type Checker |
|----------|--------|-----------|--------------|
| JavaScript | ESLint | Prettier | TypeScript |
| Python | Pylint, Flake8 | Black | mypy |
| Java | Checkstyle | Google Java Format | Built-in |
| C# | StyleCop | dotnet format | Built-in |
| Go | golint | gofmt | Built-in |

### Code Review Checklist

```
┌─────────────────────────────────────────────────────────────────┐
│               CODE REVIEW CHECKLIST                             │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  FUNCTIONALITY                                                  │
│  ☐ Code does what it's supposed to do                          │
│  ☐ Edge cases are handled                                      │
│  ☐ No obvious bugs                                             │
│                                                                 │
│  READABILITY                                                    │
│  ☐ Code is easy to understand                                  │
│  ☐ Names are clear and descriptive                             │
│  ☐ No magic numbers/strings                                    │
│                                                                 │
│  STANDARDS                                                      │
│  ☐ Follows team coding conventions                             │
│  ☐ Consistent formatting                                       │
│  ☐ Appropriate comments                                        │
│                                                                 │
│  DESIGN                                                         │
│  ☐ Single responsibility principle                             │
│  ☐ No code duplication                                         │
│  ☐ Appropriate abstraction level                               │
│                                                                 │
│  TESTING                                                        │
│  ☐ Tests are included                                          │
│  ☐ Tests cover main scenarios                                  │
│  ☐ Tests are readable                                          │
│                                                                 │
│  SECURITY                                                       │
│  ☐ No sensitive data exposed                                   │
│  ☐ Input is validated                                          │
│  ☐ No SQL injection/XSS vulnerabilities                        │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

## 📌 Key Points (Optional)
- Coding standards are like code hygiene: invisible when done well, obvious when missing
- The secret is balance: enough rules for consistency, but not so many that they paralyze development
- The best standards are those the team adopts and evolves organically, not imposed from above
- My rule: if you have to think about formatting while coding, you need better automated tooling
- Code should be written for humans, not machines
- "Any fool can write code that a computer can understand. Good programmers write code that humans can understand." - Martin Fowler

## 🖼️ Recommended Image
- Side-by-side comparison of bad vs good code formatting

## 🔗 Connections
- [[230-Definition of Done]]
- [[208-SOLID Principles]]
- [[238-Git Workflow and Version Control]]
- [[227-Sprint Backlog]]
- [[233-Velocity]]
- [[216-Non Functional Requirements]]