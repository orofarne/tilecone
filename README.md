TILECONE
========

File structure
--------------

```
 +-------+-----------------------------+
 | Index |         Binary data         |
 +-------+-----------------------------+
```

*Index*:
Array of Tile structs:
```
struct Tile {
	uint64_t offset;
	uint32_t size;
	uint32_t capacity;
};
```
