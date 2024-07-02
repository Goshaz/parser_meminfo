/*
 * parser_types.h
 *
 *  Created on: Jul 2, 2024
 *      Author: vxuser
 */

#pragma once

#include "parser_defs.h"

/*! Структура парсера (необходима для сортировки) */
typedef struct s_parser_attribute
{
	uint32_t      id						  	 ; /*! Идентификатор строки */
	char 	 	  name[ PARSER_STING_MAX_NAME_SZ]; /*! Имя файла */
	long int 	  size					  		 ; /*! Размер файла */
	char 	 	  type[PARSER_TYPE_MAX_SZ] 		 ; /*! Тип размерности файла*/
} s_parser_attribute_t;
