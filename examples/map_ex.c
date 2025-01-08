#include <stdio.h>
// #include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include <assert.h>
#include "map.h"

// array_t& test(map_t* map, size_t s) {
//   array_t *a = map_get(map, s);
//   printf("test a -----------\n");
//   pprint(a);
//   return *a;
// }


int main() {
    map_t *map = create_map(10);
    printf("map addr: %p\n", map);
    print(map);

    array_t* a = map_get(map, 8);
    array_t* aa = map_get(map, 8);
    array_t* aaa = map_get(map, 8);
    array_t* b = map_get(map, 10);
    array_t* c = map_get(map, 3);

    aa->available = true;
    aa = map_get(map, 8);
    aa->available = true;
    aa = map_get(map, 8);
    aa->available = true;
    aa = map_get(map, 8);
    for (int i=0; i<aa->size; ++i) aa->data[i] = (float)i / 2.0f;
    pprint(aa);

    pprint(b);
    pprint(c);

    // array_t v = test(map, 8);
    // print(v);
    array_t vv = *aa;
    assert(aa->data == vv.data);
    assert(aa->size == vv.size);

    print(map);

    free_map(map);
    return 0;
}