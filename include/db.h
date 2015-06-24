#ifndef TILECONE_DB
#define TILECONE_DB

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct tile {
	uint64_t offset;
	uint32_t size;
	uint32_t capacity;
};

struct __db;
typedef struct __db* db;

db new_db(const char* path, int mmappool);
db free_db(db d);
int db_ok(db d);
const char* last_error(db d);
// Returns subtiles of a tile
int get_tiles(db d, uint16_t zoom, uint64_t x, uint64_t y, void const** buf, tile const** tiles, size_t *tiles_len);
// Set new tile data. TileZoom only!
int set_tile(db d, uint64_t x, uint64_t y, void const* data, size_t data_size);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: TILECONE_DB */
