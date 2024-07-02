/*
 * main.c
 *
 *  Created on: Jul 1, 2024
 *      Author: vxuser
 */

/*! Включаемые файлы */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "parser_defs.h"
#include "parser_types.h"

/*! Определение прототипов функций */
/*! Функция сортировки, используется в qsort
 * return 0, 1, -1 как результат проверки */
static int sort_cmp( const void* a_p_arg1, const void* a_p_arg2);

/*! Описание глобальных функций */
/*! Головная функция
 * return - 0 - успех, иначе - ошибка */
int main( void)
{
	char string_tmp[ PARSER_STING_MAX_SZ] = {};
	char name_tmp  [ PARSER_STING_MAX_NAME_SZ] = {};

	long int size_tmp = 0;
	char size_type_tmp[ PARSER_TYPE_MAX_SZ] = {};
	uint32_t count = 0;

	s_parser_attribute_t *attr = NULL;
	FILE *f_src = NULL;
	int res = 0;

	f_src = fopen( PARSER_FILE_SRC_PATH,"r");

	/*! Проверка файла на корректность*/
	CHECK_FILE(f_src);

	/*! Первый проход для получения количества строк */
	while( 1)
	{
		/*! Строка может содержать не только размер, но и количество страниц
		 * Считыванием во временную строку для обработки */
		if ( NULL == fgets( string_tmp, PARSER_BUF_SZ, f_src))
		{
			break;
		}

		res = sscanf( string_tmp, "%s%ld%s", name_tmp, &size_tmp, size_type_tmp);

		/*! Исключение значений, для которых количество аргументов не равно 3
		 *  не указан размер, указано количество. Напр., HugePages_Total*/
		/*! toDo check Уточнить, как должно работать */
		if ( PARSER_ARGS_COUNT != res)
		{
			continue;
		}

		count++;
	}

	/*! Если строк нет, выход */
	if ( 0 == count)
	{
		fclose(f_src);
		return 0;
	}

	/*! Создание динамического массива */
	attr = (s_parser_attribute_t*) malloc( count * sizeof(s_parser_attribute_t));

	/*! Обнуление количества строк и переход в начало файла для второго прохода*/
	count = 0;
	rewind( f_src);

	/*! Второй проход для заполнения массива данными */
	while( 1)
	{
		if ( NULL == fgets( string_tmp, PARSER_BUF_SZ, f_src))
		{
			break;
		}

		res = sscanf( string_tmp, "%s%ld%s", name_tmp, &size_tmp, size_type_tmp);

		/*! Исключение значений, для которых количество аргументов не равно 3
		 *  не указан размер, указано количество. Напр., HugePages_Total*/
		/*! toDo check Уточнить, как должно работать */
		if ( PARSER_ARGS_COUNT != res)
		{
			continue;
		}

		/*! Заполнение массива*/
		attr[count].id = count;
		attr[count].size = size_tmp;

		size_t len = strlen(name_tmp);

		/*! Проверка, что размер имени попал в диапазон */
		assert( len <= PARSER_STING_MAX_NAME_SZ);
		/*! toDo check уточнить, как должно быть при размере имени, равном максимальному */
		if ( len > PARSER_STING_MAX_NAME_SZ)
		{
			len = PARSER_STING_MAX_NAME_SZ;
		}

		/*! Удаление символа : */
		memcpy( attr[count].name, name_tmp, 	 len - 1);
		/*! toDo Для размерности файла всегда используется kB. Уточнить, как должно быть*/
		memcpy( attr[count].type, size_type_tmp, PARSER_TYPE_MAX_SZ);
		count++;
	}
	/*! Закрытие файла */
	fclose(f_src);

#ifdef PRINTF_RES
	for( uint32_t idx = 0; idx < count; idx++)
	{
		printf("[%d] %s %ld  %s \n",
				attr[idx].id, attr[idx].name, attr[idx].size, attr[idx].type);
	}
#endif

	/*! toDO добавить варианты сортировки: по имени */
	/*! toDO добавить типы сортировки: по возрастанию, по убыванию */
	/*! Непосредственно сортировка */
	qsort( attr, count, sizeof(s_parser_attribute_t), sort_cmp);

#ifdef PRINTF_RES
	for( uint32_t idx = 0; idx < count; idx++)
	{
		printf("[%d]  %s!%ld! %s \n",
				attr[idx].id, attr[idx].name, attr[idx].size, attr[idx].type);
	}
#endif

	/*! Конечный файл*/
	FILE *f_dst = NULL;
	f_dst = fopen( PARSER_FILE_DST_PATH,"w");

	/*! Проверка файла на корректность */
	CHECK_FILE(f_dst);

	/*! Проход по всем элементам массива */
	for( int idx = 0; idx < count; idx++)
	{

		/*! В файл выдаётся "-" вместо 0 в размере */
		if ( 0 == attr[ idx].size )
		{
			fprintf( f_dst, "%s %c %s\n", attr[ idx].name, '-', attr[ idx].type);
		}
		else
		{
			fprintf( f_dst, "%s %ld %s\n", attr[ idx].name, attr[ idx].size, attr[ idx].type);
		}
	}

	/*! Закрытие файла*/
	fclose( f_dst);
	/*! Освобождение динамической памяти */
	free( attr);

	return 0;
}

/*! Функция сортировки, используется в qsort
 * return 0, 1, -1 как результат проверки */
static int sort_cmp( const void* a_p_arg1, const void* a_p_arg2)
{
	s_parser_attribute_t* attr1 = (s_parser_attribute_t*) a_p_arg1;
	s_parser_attribute_t* attr2 = (s_parser_attribute_t*) a_p_arg2;

	if ( attr1->size == attr2->size)
	{
		return 0;
	}
	else if ( attr1->size < attr2->size)
	{
		return -1;
	}
	else if ( attr1->size > attr2->size)
	{
		return 1;
	}

	return 0;
}
