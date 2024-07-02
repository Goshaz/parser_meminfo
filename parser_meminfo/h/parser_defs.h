/*
 * parser_defs.h
 *
 *  Created on: Jul 2, 2024
 *      Author: vxuser
 */

#pragma once

/*! Макросы */
#define PARSER_STING_MAX_SZ			( 256U)	/*!< Максимальный размер строки */
#define PARSER_STING_MAX_NAME_SZ	( 40U)	/*!< Максимальный размер имени */
#define PARSER_TYPE_MAX_SZ			( 4U)	/*!< Максимальный размер количества данных */

/*! Работа с тестовым файлом (отладка)*/
#undef PARSER_TEST

/*! Имена файлов: исходный, конечный */
#ifdef PARSER_TEST
#define PARSER_FILE_SRC_PATH ( "/home/vxuser/meminfo")
#else
#define PARSER_FILE_SRC_PATH ( "/proc/meminfo")
#endif
#define PARSER_FILE_DST_PATH "/home/vxuser/meminfo_sort"

/*! Размер буфера для чтения из файла*/
#define PARSER_BUF_SZ (PARSER_STING_MAX_SZ)

/*! Количество аргументов для парсинга*/
#define PARSER_ARGS_COUNT (3)

/*! Отладочный вывод на экран */
#undef PRINTF_RES

/*! Объединение строк */
#define CONCAT(M_A_ARG1, M_A_ARG2)	\
		M_A_ARG1#M_A_ARG2			\

/*! Проверка файла на корректность */
#define CHECK_FILE(M_A_NAME)								\
	assert( NULL != M_A_NAME);								\
	if ( NULL == M_A_NAME)									\
	{														\
		printf( "%s\n", CONCAT("FILE ERROR ", M_A_NAME));	\
		return 0;											\
	}
