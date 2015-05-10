namespace cpp tilecone

struct Tile {
	1: i64 offset,
	2: i64 size,
	3: i64 capacity
}

struct IndexFile {
	1: i16 min_zoom,
	2: i16 max_zoom,
	3: i64 coord_x,
	4: i64 coord_y,
	5: list<Tile> tiles
}
