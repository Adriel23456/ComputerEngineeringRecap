---
Theme: Compression Algorithms
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Algorithms, Compression, DataStructures]
---

## 📚 Idea/Concept

Data compression reduces file size by encoding information more efficiently. Compression algorithms fall into two categories: lossless (perfect reconstruction) and lossy (acceptable quality loss). Understanding compression is essential for storage optimization, network transmission, and multimedia processing.

### Compression Types
```
Compression
├── Lossless (exact reconstruction)
│   ├── Huffman Coding
│   ├── LZW (GIF, ZIP)
│   ├── DEFLATE (ZIP, PNG, gzip)
│   ├── Run-Length Encoding
│   └── Arithmetic Coding
│
└── Lossy (quality loss acceptable)
    ├── JPEG (images)
    ├── MP3, AAC (audio)
    ├── H.264, H.265 (video)
    └── Perceptual coding
```

**Compression Ratio**:
```
Ratio = Original Size / Compressed Size

Example: 1 MB → 250 KB
Ratio = 1000 / 250 = 4:1 (or 75% reduction)
```

---

### Run-Length Encoding (RLE)

Simplest compression: replace repeated sequences with count + value.
```
Original: AAAAAABBBCCCCCCCCDDDD
Encoded:  6A3B8C4D

Original: 21 characters
Encoded:  8 characters
Ratio: 2.6:1

// Works well for:
// - Simple graphics with solid colors
// - Binary images (fax machines)

// Works poorly for:
// - Text (few repetitions)
// - Photographs (varied pixels)
```

**Implementation**:
```python
def rle_encode(data):
    encoding = []
    i = 0
    while i < len(data):
        count = 1
        while i + count < len(data) and data[i + count] == data[i]:
            count += 1
        encoding.append((count, data[i]))
        i += count
    return encoding

def rle_decode(encoded):
    return ''.join(char * count for count, char in encoded)
```

---

### Huffman Coding

Variable-length encoding: frequent symbols get shorter codes.

**Key Insight**: In ASCII, every character is 8 bits. But if 'e' appears 1000 times and 'z' appears once, why give them equal space?

**Algorithm**:
```
1. Count frequency of each symbol
2. Create leaf node for each symbol
3. Repeat until one tree:
   a. Take two nodes with lowest frequency
   b. Create parent with combined frequency
   c. Left edge = 0, Right edge = 1
4. Code = path from root to leaf
```

**Example**:
```
Text: "ABRACADABRA"

Frequencies:
  A: 5, B: 2, R: 2, C: 1, D: 1

Build Tree:
        (11)
       /    \
     (6)    A(5)
    /    \
  (3)    (3)
  / \    / \
C(1)D(1)B(2)R(2)

Huffman Codes:
  A: 1      (1 bit)
  B: 010   (3 bits)
  R: 011   (3 bits)
  C: 000   (3 bits)
  D: 001   (3 bits)

Original: 11 chars × 8 bits = 88 bits
Encoded: 5×1 + 2×3 + 2×3 + 1×3 + 1×3 = 23 bits
Compression: 74% reduction
```

**Implementation**:
```python
import heapq
from collections import Counter

class HuffmanNode:
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None
    
    def __lt__(self, other):
        return self.freq < other.freq

def build_huffman_tree(text):
    # Count frequencies
    freq = Counter(text)
    
    # Create priority queue of nodes
    heap = [HuffmanNode(char, f) for char, f in freq.items()]
    heapq.heapify(heap)
    
    # Build tree
    while len(heap) > 1:
        left = heapq.heappop(heap)
        right = heapq.heappop(heap)
        
        merged = HuffmanNode(None, left.freq + right.freq)
        merged.left = left
        merged.right = right
        
        heapq.heappush(heap, merged)
    
    return heap[0]

def generate_codes(node, code="", codes={}):
    if node.char is not None:
        codes[node.char] = code
        return codes
    
    generate_codes(node.left, code + "0", codes)
    generate_codes(node.right, code + "1", codes)
    return codes
```

---

### LZW (Lempel-Ziv-Welch)

Dictionary-based compression: build dictionary of repeated patterns during encoding.

**Key Insight**: Don't just compress single characters—compress repeated sequences.

**Algorithm**:
```
Initialize dictionary with single characters (0-255)
current = first character
while characters remain:
    next = next character
    if (current + next) in dictionary:
        current = current + next
    else:
        output dictionary[current]
        add (current + next) to dictionary
        current = next
output dictionary[current]
```

**Example**:
```
Input: "ABABABA"

Initial Dictionary: {A:0, B:1}

Step | Current | Next | Output | Add to Dict
-----|---------|------|--------|-------------
1    | A       | B    | 0      | AB:2
2    | B       | A    | 1      | BA:3
3    | A       | B    | -      | (AB exists)
4    | AB      | A    | 2      | ABA:4
5    | A       | B    | -      | (AB exists)
6    | AB      | A    | -      | (ABA exists)
7    | ABA     | -    | 4      | -

Output: [0, 1, 2, 4]

Original: 7 symbols
Compressed: 4 codes
```

**Used In**: GIF, TIFF, PDF, Unix compress

---

### Lossy Compression Concepts

**JPEG Image Compression**:
```
1. Color Space: RGB → YCbCr (luminance + chrominance)
2. Downsampling: Reduce chrominance resolution (eye less sensitive)
3. DCT: Discrete Cosine Transform (frequency domain)
4. Quantization: Reduce precision (this loses data!)
5. Huffman/Arithmetic: Encode the result

Quality setting controls quantization aggressiveness
```

**Perceptual Coding (Audio)**:
```
Psychoacoustic Model:
- Masking: Loud sounds hide quiet nearby frequencies
- Hearing range: ~20 Hz - 20 kHz (less sensitive at extremes)
- Temporal masking: Brief sounds after loud ones inaudible

MP3 removes "inaudible" frequencies based on these models
```

---

### Comparison

| Algorithm | Type | Ratio | Speed | Use Case |
|-----------|------|-------|-------|----------|
| RLE | Lossless | 2-10:1* | Very fast | Simple graphics |
| Huffman | Lossless | 1.5-3:1 | Fast | Text, general |
| LZW | Lossless | 2-5:1 | Medium | GIF, general |
| DEFLATE | Lossless | 2-8:1 | Medium | ZIP, PNG |
| JPEG | Lossy | 10-100:1 | Medium | Photographs |
| MP3 | Lossy | 10-12:1 | Medium | Music |

*Highly data-dependent

### When to Use What
```
Text/Code/Documents?
  → Lossless (ZIP, gzip, DEFLATE)

Images - need perfect quality?
  → PNG (lossless)

Images - photographs?
  → JPEG (lossy, good ratio)

Audio - archival?
  → FLAC (lossless)

Audio - streaming/storage?
  → MP3, AAC (lossy)

Need to decompress frequently?
  → Prefer faster algorithms (LZ4 over LZMA)
```

## 📌 Key Points (Optional)
- Lossless: Perfect reconstruction (Huffman, LZW, DEFLATE)
- Lossy: Acceptable quality loss for better ratios (JPEG, MP3)
- Huffman: Frequent symbols get shorter codes
- LZW: Dictionary-based, learns patterns during compression
- Choose based on data type and quality requirements

## 🖼️ Recommended Image
- Huffman tree construction with resulting codes shown

## 🔗 Connections
- [[171-Data Structures Overview]]
- [[172-Binary Trees and Traversals]]
- [[173-Heap Data Structure]]
- [[176-Algorithm Complexity Analysis]]