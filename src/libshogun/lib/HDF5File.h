/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2010 Soeren Sonnenburg
 * Copyright (C) 2010 Berlin Institute of Technology
 */
#ifndef __HDF5_FILE_H__
#define __HDF5_FILE_H__

#include <shogun/lib/config.h>

#ifdef HAVE_HDF5
#include <shogun/lib/common.h>
#include <shogun/lib/io.h>
#include <shogun/lib/File.h>
#include <shogun/base/SGObject.h>
#include <hdf5.h>


namespace shogun
{
template <class ST> struct T_STRING;
template <class ST> struct TSparse;

/** @brief A HDF5File access class.
 *
 *
 */
class CHDF5File : public CFile
{
public:
	/** constructor
	 *
	 * @param fname filename to open
	 * @param rw mode, 'r' or 'w'
	 * @param name variable name (e.g. "x" or "/path/to/x")
	 */
	CHDF5File(char* fname, char rw='r', const char* name=NULL);

	virtual ~CHDF5File();

	/** get data type of current element */
	/*virtual DataType get_data_type();*/

	/** vector access functions */
	/*virtual void get_vector(void*& vector, int32_t& len, DataType& dtype);*/

	virtual void get_bool_vector(bool*& vector, int32_t& len);
	virtual void get_byte_vector(uint8_t*& vector, int32_t& len);
	virtual void get_char_vector(char*& vector, int32_t& len);
	virtual void get_int_vector(int32_t*& vector, int32_t& len);
	virtual void get_real_vector(float64_t*& vector, int32_t& len);
	virtual void get_shortreal_vector(float32_t*& vector, int32_t& len);
	virtual void get_short_vector(int16_t*& vector, int32_t& len);
	virtual void get_word_vector(uint16_t*& vector, int32_t& len);

	/** matrix access functions */
	/*virtual void get_matrix(
			void*& matrix, int32_t& num_feat, int32_t& num_vec, DataType& dtype);*/

	virtual void get_bool_matrix(
			bool*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_byte_matrix(
			uint8_t*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_char_matrix(
			char*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_int_matrix(
			int32_t*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_uint_matrix(
			uint32_t*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_long_matrix(
			int64_t*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_ulong_matrix(
			uint64_t*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_shortreal_matrix(
			float32_t*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_real_matrix(
			float64_t*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_longreal_matrix(
			floatmax_t*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_short_matrix(
			int16_t*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_word_matrix(
			uint16_t*& matrix, int32_t& num_feat, int32_t& num_vec);

	/** nd-array access functions */
	/*virtual void get_ndarray(
			void*& array, int32_t*& dims, int32_t& num_dims, DataType& dtype);*/

	virtual void get_byte_ndarray(
			uint8_t*& array, int32_t*& dims, int32_t& num_dims);
	virtual void get_char_ndarray(
			char*& array, int32_t*& dims, int32_t& num_dims);
	virtual void get_int_ndarray(
			int32_t*& array, int32_t*& dims, int32_t& num_dims);
	virtual void get_shortreal_ndarray(
			float32_t*& array, int32_t*& dims, int32_t& num_dims);
	virtual void get_real_ndarray(
			float64_t*& array, int32_t*& dims, int32_t& num_dims);
	virtual void get_short_ndarray(
			int16_t*& array, int32_t*& dims, int32_t& num_dims);
	virtual void get_word_ndarray(
			uint16_t*& array, int32_t*& dims, int32_t& num_dims);

	virtual void get_bool_sparsematrix(
			TSparse<bool>*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_byte_sparsematrix(
			TSparse<uint8_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_char_sparsematrix(
			TSparse<char>*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_int_sparsematrix(
			TSparse<int32_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_uint_sparsematrix(
			TSparse<uint32_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_long_sparsematrix(
			TSparse<int64_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_ulong_sparsematrix(
			TSparse<uint64_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_short_sparsematrix(
			TSparse<int16_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_word_sparsematrix(
			TSparse<uint16_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_shortreal_sparsematrix(
			TSparse<float32_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_real_sparsematrix(
			TSparse<float64_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
	virtual void get_longreal_sparsematrix(
			TSparse<floatmax_t>*& matrix, int32_t& num_feat, int32_t& num_vec);


	virtual void get_bool_string_list(
			T_STRING<bool>*& strings, int32_t& num_str,
			int32_t& max_string_len);
	virtual void get_byte_string_list(
			T_STRING<uint8_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
	virtual void get_char_string_list(
			T_STRING<char>*& strings, int32_t& num_str,
			int32_t& max_string_len);
	virtual void get_int_string_list(
			T_STRING<int32_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
	virtual void get_uint_string_list(
			T_STRING<uint32_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
	virtual void get_short_string_list(
			T_STRING<int16_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
	virtual void get_word_string_list(
			T_STRING<uint16_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
	virtual void get_long_string_list(
			T_STRING<int64_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
	virtual void get_ulong_string_list(
			T_STRING<uint64_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
	virtual void get_shortreal_string_list(
			T_STRING<float32_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
	virtual void get_real_string_list(
			T_STRING<float64_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
	virtual void get_longreal_string_list(
			T_STRING<floatmax_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);

	virtual void set_bool_vector(const bool* vector, int32_t len);
	virtual void set_byte_vector(const uint8_t* vector, int32_t len);
	virtual void set_char_vector(const char* vector, int32_t len);
	virtual void set_int_vector(const int32_t* vector, int32_t len);
	virtual void set_shortreal_vector(
			const float32_t* vector, int32_t len);
	virtual void set_real_vector(const float64_t* vector, int32_t len);
	virtual void set_short_vector(const int16_t* vector, int32_t len);
	virtual void set_word_vector(const uint16_t* vector, int32_t len);


	virtual void set_bool_matrix(
			const bool* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_byte_matrix(
			const uint8_t* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_char_matrix(
			const char* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_int_matrix(
			const int32_t* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_uint_matrix(
			const uint32_t* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_long_matrix(
			const int64_t* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_ulong_matrix(
			const uint64_t* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_shortreal_matrix(
			const float32_t* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_real_matrix(
			const float64_t* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_longreal_matrix(
			const floatmax_t* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_short_matrix(
			const int16_t* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_word_matrix(
			const uint16_t* matrix, int32_t num_feat, int32_t num_vec);

	virtual void set_bool_sparsematrix(
			const TSparse<bool>* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_byte_sparsematrix(
			const TSparse<uint8_t>* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_char_sparsematrix(
			const TSparse<char>* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_int_sparsematrix(
			const TSparse<int32_t>* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_uint_sparsematrix(
			const TSparse<uint32_t>* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_long_sparsematrix(
			const TSparse<int64_t>* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_ulong_sparsematrix(
			const TSparse<uint64_t>* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_short_sparsematrix(
			const TSparse<int16_t>* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_word_sparsematrix(
			const TSparse<uint16_t>* matrix, int32_t num_feat, int32_t num_vec); 
	virtual void set_shortreal_sparsematrix(
			const TSparse<float32_t>* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_real_sparsematrix(
			const TSparse<float64_t>* matrix, int32_t num_feat, int32_t num_vec);
	virtual void set_longreal_sparsematrix(
			const TSparse<floatmax_t>* matrix, int32_t num_feat, int32_t num_vec);


	virtual void set_bool_string_list(
			const T_STRING<bool>* strings, int32_t num_str);
	virtual void set_byte_string_list(
			const T_STRING<uint8_t>* strings, int32_t num_str);
	virtual void set_char_string_list(
			const T_STRING<char>* strings, int32_t num_str);
	virtual void set_int_string_list(
			const T_STRING<int32_t>* strings, int32_t num_str);
	virtual void set_uint_string_list(
			const T_STRING<uint32_t>* strings, int32_t num_str);
	virtual void set_short_string_list(
			const T_STRING<int16_t>* strings, int32_t num_str);
	virtual void set_word_string_list(
			const T_STRING<uint16_t>* strings, int32_t num_str);
	virtual void set_long_string_list(
			const T_STRING<int64_t>* strings, int32_t num_str);
	virtual void set_ulong_string_list(
			const T_STRING<uint64_t>* strings, int32_t num_str);
	virtual void set_shortreal_string_list(
			const T_STRING<float32_t>* strings, int32_t num_str);
	virtual void set_real_string_list(
			const T_STRING<float64_t>* strings, int32_t num_str);
	virtual void set_longreal_string_list(
			const T_STRING<floatmax_t>* strings, int32_t num_str);

	/** @return object name */
	inline virtual const char* get_name() const { return "HDF5File"; }
protected:
	void get_boolean_type();

protected:
	hid_t h5file;
	hid_t boolean_type;
};
}
#endif //  HDF5
#endif //__HDF5_FILE_H__

