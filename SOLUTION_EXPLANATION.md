# Great Grid Problem - Solution Explanation

## Problem Summary

Construct a grid where:
1. Each cell contains 'A', 'B', or 'C'
2. Every 2×2 subgrid contains all three letters
3. Adjacent cells (sharing an edge) have different letters

Given k constraints where diagonal cells `(x1,y1)` and `(x1+1, y1±1)` must have the same letter, determine if a valid grid exists.

## Key Insight: Grid Structure

Any valid "great grid" can be represented as:

```
cell(i, j) = (f(i) + g(j)) mod 3
```

Where:
- `f: rows → {0, 1, 2}` with `f(i) ≠ f(i+1)` for all i
- `g: columns → {0, 1, 2}` with `g(j) ≠ g(j+1)` for all j
- Values 0, 1, 2 map to letters A, B, C

### Why This Works

**Edge constraint:** Adjacent cells differ because:
- Horizontal: `f(i) + g(j) ≠ f(i) + g(j+1)` since `g(j) ≠ g(j+1)`
- Vertical: `f(i) + g(j) ≠ f(i+1) + g(j)` since `f(i) ≠ f(i+1)`

**2×2 constraint:** For any 2×2 subgrid, the four values are:
```
f(i)+g(j),     f(i)+g(j)+Δg,
f(i)+Δf+g(j),  f(i)+Δf+g(j)+Δg
```
Where Δf, Δg ∈ {1, 2}. The set {0, Δf, Δg, Δf+Δg} (mod 3) always contains all three values 0, 1, 2.

## Reduction to 2-SAT

Since consecutive values must differ:
- `Δf(i) = f(i+1) - f(i) ∈ {1, 2}` — binary choice!
- `Δg(j) = g(j+1) - g(j) ∈ {1, 2}` — binary choice!

Define binary variables:
- `r[i] = 1` if `Δf(i) = 1`, else `r[i] = 0` (meaning `Δf(i) = 2`)
- `c[j] = 1` if `Δg(j) = 1`, else `c[j] = 0`

### Translating Diagonal Constraints

For constraint: `cell(x1, y1) = cell(x2, y2)`

**Case 1: `y2 = y1 + 1`** (right-down diagonal)
```
f(x1) + g(y1) ≡ f(x1+1) + g(y1+1) (mod 3)
-Δf(x1) ≡ Δg(y1) (mod 3)
```
Since Δf, Δg ∈ {1, 2}, this means `Δf(x1) + Δg(y1) ≡ 0 (mod 3)`:
- If one is 1, the other must be 2
- Therefore: **r[x1] ≠ c[y1]**

**Case 2: `y2 = y1 - 1`** (left-down diagonal)
```
f(x1) + g(y1) ≡ f(x1+1) + g(y1-1) (mod 3)
Δf(x1) ≡ Δg(y1-1) (mod 3)
```
Therefore: **r[x1] = c[y1-1]**

## Graph 2-Coloring Problem

We now have:
- Variables: `r[1], ..., r[n-1]` and `c[1], ..., c[m-1]`
- Constraints: pairs must be equal or different

This is a **graph 2-coloring** problem:
1. Create a node for each variable
2. Add edges labeled "same" or "different"
3. Check if consistent 2-coloring exists

### Algorithm

```
For each connected component:
    BFS/DFS assigning colors 0 or 1
    For edge (u, v) with label "same": color[v] = color[u]
    For edge (u, v) with label "different": color[v] = 1 - color[u]
    If conflict found → NO
If all components colored successfully → YES
```

## Complexity

- **Time:** O(n + m + k) per test case
- **Space:** O(n + m + k)

## Code Walkthrough

```cpp
int total = (n - 1) + (m - 1);  // Total variables
vector<vector<pair<int, int>>> adj(total);  // Adjacency list with edge types
```

Node mapping:
- `r[i]` → node `i - 1` (indices 0 to n-2)
- `c[j]` → node `(n-1) + (j-1)` (indices n-1 to n+m-3)

For each constraint:
```cpp
if (y2 == y1 + 1) {
    cNode = (n - 1) + (y1 - 1);  // c[y1]
    edgeType = 1;                 // different
} else {
    cNode = (n - 1) + (y1 - 2);  // c[y1-1]
    edgeType = 0;                 // same
}
```

BFS coloring:
```cpp
int expected = (type == 0) ? color[u] : (1 - color[u]);
if (color[v] == -1) {
    color[v] = expected;  // Assign color
} else if (color[v] != expected) {
    possible = false;     // Conflict!
}
```

## Example Trace

**Test 2:** n=2, m=7, k=2
- Constraint (1,1)→(2,2): `r[1] ≠ c[1]`
- Constraint (1,2)→(2,1): `r[1] = c[1]`

Both constraints involve `r[1]` and `c[1]` but require opposite relationships → **Contradiction → NO**

**Test 1:** n=3, m=4, k=4
- `r[1] ≠ c[1]`
- `r[2] ≠ c[1]`
- `r[1] = c[3]`
- `r[2] = c[2]`

Assigning `r[1] = 0`:
- `c[1] = 1` (from ≠)
- `r[2] = 0` (from r[2] ≠ c[1] = 1)
- `c[3] = 0` (from =)
- `c[2] = 0` (from =)

No conflicts → **YES**

