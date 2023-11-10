//
//  smm_database.h
//  infection Path
//
//  Created by Juyeop Kim on 2023/11/05.
//

#ifndef smm_database_h
#define smm_database_h

#define LISTNO_NODE             0
#define LISTNO_FOODCARD         1
#define LISTNO_FESTCARD         2
#define LISTNO_OFFSET_GRADE     3

int smmdb_addTail(int list_nr, void* obj);          //add data to tail
int smmdb_deleteData(int list_nr, int index);       //delete data
int smmdb_len(int list_nr);                   //get database length
void* smmdb_getData(int list_nr, int index);        //get index'th data

#endif /* smm_database_h */
