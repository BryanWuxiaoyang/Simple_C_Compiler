#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <cassert>

using namespace std;

#define make_pool_handler(struct_type) \
struct _Handler_##struct_type##_ { \
	struct struct_type elem; \
	int refCnt; \
	struct _Handler_##struct_type##_* next; \
}; \
typedef struct _Handler_##struct_type##_* Handler_##struct_type##;

#define make_pool_handler_variable(struct_type) \
Handler_##struct_type availHandler_##struct_type##;

#define make_pool_block(struct_type, block_capacity) \
struct _Block_##struct_type##_ { \
	struct _Handler_##struct_type##_ handlerList[block_capacity]; \
	struct _Block_##struct_type##_* next; \
}; \
typedef struct _Block_##struct_type##_* Block_##struct_type##;

#define make_pool_block_variables(struct_type) \
Block_##struct_type blockhead_##struct_type##; \
Block_##struct_type blocktail_##struct_type##; 

#define make_pool_handler_getter(struct_type) \
struct struct_type* get_##struct_type##(Handler_##struct_type handler){ \
	if(handler->refCnt <= 0) assert(0 && "element not exist"); \
	else return &handler->elem; \
	return NULL; \
}

#define make_pool_destroy_handler(struct_type, clear_func) \
void destroy_handler_##struct_type##(Handler_##struct_type handler){ \
	handler->refCnt--; \
	if (handler->refCnt <= 0) { \
		handler->next = availHandler_##struct_type##; \
		clear_func (&handler->elem); \
		availHandler_##struct_type = handler; \
	}\
}

#define make_pool_copy_handler(struct_type)\
Handler_##struct_type copy_handler_##struct_type##(Handler_##struct_type handler){ \
	handler->refCnt++; \
	return handler; \
}

#define make_pool_createBlock(struct_type, block_capacity) \
Block_##struct_type createBlock_##struct_type##() { \
	Block_##struct_type block = (Block_##struct_type##)malloc(sizeof(struct _Block_##struct_type##_)); \
	if (block) { \
		block->next = NULL; \
		for (int i = 0; i < block_capacity - 1; i++) { \
			block->handlerList[i].next = &block->handlerList[i + 1]; \
		} \
		block->handlerList[block_capacity - 1].next = NULL; \
	} \
	return block; \
}

#define make_pool_checkPoolInit(struct_type) \
void checkPoolInit_##struct_type##() { \
	if (blockhead_##struct_type == NULL) { \
		blockhead_##struct_type = createBlock_##struct_type##(); \
		blocktail_##struct_type = blockhead_##struct_type##; \
		availHandler_##struct_type = &blockhead_##struct_type##->handlerList[0]; \
	} \
}

#define make_pool_create(struct_type, init_func) \
Handler_##struct_type create_##struct_type##() { \
	checkPoolInit_##struct_type##(); \
	if (availHandler_##struct_type##->next == NULL) { \
		Block_##struct_type newBlock = createBlock_##struct_type##(); \
		blocktail_##struct_type##->next = newBlock; \
		blocktail_##struct_type = newBlock; \
		availHandler_##struct_type##->next = &newBlock->handlerList[0]; \
	} \
	Handler_##struct_type handler = availHandler_##struct_type##; \
	availHandler_##struct_type = availHandler_##struct_type##->next; \
	handler->refCnt = 1; \
	init_func (&handler->elem); \
	return handler; \
}

#define make_pool_print_status(struct_type, block_capacity) \
void print_status_##struct_type##(){ \
	if(blockhead_Type == NULL){ printf("empty pool\n"); return; } \
	Block_##struct_type block = blockhead_##struct_type##; \
	int cnt = 0; \
	while (block) { \
		printf("block %d:\n", ++cnt); \
		for (int i = 0; i < block_capacity; i++) \
			printf("\tno:%d, refCnt:%d\n", i, block->handlerList[i].refCnt); \
		block = block->next; \
	} \
}

#define make_pool_destroy_pool(struct_type, block_capacity, clear_func) \
void destroy_pool_##struct_type##(){ \
	Block_##struct_type block = blockhead_##struct_type##; \
	while(block){ \
		for(int i = 0; i < block_capacity; i++){ \
			if(block->handlerList[i].refCnt > 0) \
				clear_func (&block->handlerList[i].elem); \
		} \
		Block_##struct_type prevBlock = block; \
		block = block->next; \
		free(prevBlock); \
	} \
	blockhead_##struct_type = blocktail_##struct_type = NULL; \
	availHandler_##struct_type = NULL; \
}

#define make_pool(struct_type, block_capacity, init_func, clear_func) \
make_pool_handler(struct_type); \
make_pool_handler_variable(struct_type); \
make_pool_handler_getter(struct_type); \
make_pool_destroy_handler(struct_type, clear_func); \
make_pool_copy_handler(struct_type); \
make_pool_block(struct_type, block_capacity); \
make_pool_block_variables(struct_type); \
make_pool_print_status(struct_type, block_capacity); \
make_pool_createBlock(struct_type, block_capacity); \
make_pool_checkPoolInit(struct_type); \
make_pool_create(struct_type, init_func);\
make_pool_destroy_pool(struct_type, block_capacity, clear_func);

struct Type{
	int a;
	char* name;
}

void init(struct Type* type){
	type->a;
}
void clear(struct Type* type){
	free(type->name);
} 

make_pool(Type, 100, init, clear);
void use(){
	Handler_Type handler = create_Type();
	get_Type(handler)->a = 1;
	copy_handler_Type(handler);
	destroy_handler_Type(handler);
	return ;
}