#ifndef TILECONE_DB
#define TILECONE_DB

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct tc_tile {
	uint64_t offset;
	uint32_t size;
	uint32_t capacity;
};

struct __tc_db;
typedef struct __tc_db* tc_db;

tc_db tc_new_db(const char* path, int mmappool);
tc_db tc_free_db(tc_db d);
int tc_db_ok(tc_db d);
const char* tc_last_error(tc_db d);
// Returns subtiles of a tile
int tc_get_tiles(tc_db d, uint16_t zoom, uint64_t x, uint64_t y, void const** buf, struct tc_tile const** tiles, size_t *tiles_len);
// Set new tile data. TileZoom only!
int tc_set_tile(tc_db d, uint64_t x, uint64_t y, void const* data, size_t data_size);

// Info:
uint64_t tc_bucket_zoom(tc_db d);
uint64_t tc_tile_zoom(tc_db d);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: TILECONE_DB */
