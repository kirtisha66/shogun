/*
 * This software is distributed under BSD 3-clause license (see LICENSE file).
 *
 * Authors: Sergey Lisitsyn
 */

#include <shogun/features/hashed/HashedDenseFeatures.h>
#include <shogun/lib/Hash.h>
#include <shogun/lib/DynamicArray.h>
#include <shogun/io/SGIO.h>
#include <shogun/mathematics/Math.h>

#include <shogun/lib/SGVector.h>
#include <shogun/lib/SGSparseVector.h>

#include <string.h>

namespace shogun {
template <class ST>
HashedDenseFeatures<ST>::HashedDenseFeatures(int32_t size, bool use_quadr, bool keep_lin_terms)
: DotFeatures(size)
{
	init(NULL, 0, use_quadr, keep_lin_terms);
}

template <class ST>
HashedDenseFeatures<ST>::HashedDenseFeatures(std::shared_ptr<DenseFeatures<ST>> feats, int32_t d,
	bool use_quadr, bool keep_lin_terms) : DotFeatures()
{
	init(feats, d, use_quadr, keep_lin_terms);
}

template <class ST>
HashedDenseFeatures<ST>::HashedDenseFeatures(SGMatrix<ST> matrix, int32_t d, bool use_quadr,
	bool keep_lin_terms) : DotFeatures()
{
	auto feats = std::make_shared<DenseFeatures<ST>>(matrix);
	init(feats, d, use_quadr, keep_lin_terms);
}

template <class ST>
HashedDenseFeatures<ST>::HashedDenseFeatures(ST* src, int32_t num_feat, int32_t num_vec,
	int32_t d, bool use_quadr, bool keep_lin_terms) : DotFeatures()
{
	auto feats = std::make_shared<DenseFeatures<ST>>(src, num_feat, num_vec);
	init(feats, d, use_quadr, keep_lin_terms);
}

template <class ST>
HashedDenseFeatures<ST>::HashedDenseFeatures(std::shared_ptr<File> loader, int32_t d, bool use_quadr,
	bool keep_lin_terms) : DotFeatures(loader)
{
	auto feats = std::make_shared<DenseFeatures<ST>>();
	feats->load(loader);
	init(feats, d, use_quadr, keep_lin_terms);
}

template <class ST>
void HashedDenseFeatures<ST>::init(std::shared_ptr<DenseFeatures<ST>> feats, int32_t d, bool use_quadr,
	bool keep_lin_terms)
{
	dim = d;
	dense_feats = feats;

	use_quadratic = use_quadr;
	keep_linear_terms = keep_lin_terms;

	SG_ADD(&use_quadratic, "use_quadratic", "Whether to use quadratic features");
	SG_ADD(&keep_linear_terms, "keep_linear_terms", "Whether to keep the linear terms or not");
	SG_ADD(&dim, "dim", "Dimension of new feature space");
	SG_ADD((std::shared_ptr<SGObject>* ) &dense_feats, "dense_feats", "Dense features to work on");

	set_generic<ST>();
}

template <class ST>
HashedDenseFeatures<ST>::HashedDenseFeatures(const HashedDenseFeatures& orig)
: DotFeatures(orig)
{
	init(orig.dense_feats, orig.dim, orig.use_quadratic, orig.keep_linear_terms);
}

template <class ST>
HashedDenseFeatures<ST>::~HashedDenseFeatures()
{

}

template <class ST>
std::shared_ptr<Features> HashedDenseFeatures<ST>::duplicate() const
{
	return std::make_shared<HashedDenseFeatures>(*this);
}

template <class ST>
int32_t HashedDenseFeatures<ST>::get_dim_feature_space() const
{
	return dim;
}

template <class ST>
float64_t HashedDenseFeatures<ST>::dot(int32_t vec_idx1, std::shared_ptr<DotFeatures> df,
	int32_t vec_idx2) const
{
	ASSERT(df)
	ASSERT(df->get_feature_type() == get_feature_type())
	ASSERT(df->get_feature_class() == get_feature_class())
	ASSERT(strcmp(df->get_name(), get_name())==0)

	auto feats = std::static_pointer_cast<HashedDenseFeatures<ST>>(df);
	ASSERT(feats->get_dim_feature_space() == get_dim_feature_space())

	SGSparseVector<ST> vec_1 = get_hashed_feature_vector(vec_idx1);

	bool same_vec = (df.get() == this) && (vec_idx1 == vec_idx2);
	SGSparseVector<ST> vec_2 = same_vec ? vec_1 : feats->get_hashed_feature_vector(vec_idx2);
	float64_t result = vec_1.sparse_dot(vec_2);

	return result;
}

template <class ST>
float64_t HashedDenseFeatures<ST>::dot(
	int32_t vec_idx1, const SGVector<float64_t>& vec2) const
{
	ASSERT(vec2.size() == dim)

	SGVector<ST> vec = dense_feats->get_feature_vector(vec_idx1);

	float64_t result = 0;

	int32_t hash_cache_size = use_quadratic ? vec.size() : 0;
	SGVector<uint32_t> hash_cache(hash_cache_size);

	for (index_t i=0; i<vec.size(); i++)
	{
		uint32_t h_idx = Hash::MurmurHash3((uint8_t* ) &i, sizeof (index_t), i);
		if (use_quadratic)
			hash_cache[i] = h_idx;

		if ( (!use_quadratic) || keep_linear_terms)
			result += vec2[h_idx % dim] * vec[i];
	}

	if (use_quadratic)
	{
		for (index_t i=0; i<vec.size(); i++)
		{
			int32_t n_idx = i * vec.size() + i;
			uint32_t idx = Hash::MurmurHash3((uint8_t* ) &n_idx, sizeof (index_t), n_idx) % dim;
			result += vec2[idx] * vec[i] * vec[i];

			for (index_t j=i+1; j<vec.size(); j++)
			{
				idx = (hash_cache[i] ^ hash_cache[j]) % dim;
				result += vec2[idx] * vec[i] * vec[j];
			}
		}
	}

	dense_feats->free_feature_vector(vec, vec_idx1);
	return result;
}

template <class ST>
void HashedDenseFeatures<ST>::add_to_dense_vec(float64_t alpha, int32_t vec_idx1,
	float64_t* vec2, int32_t vec2_len, bool abs_val) const
{
	float64_t val = abs_val ? Math::abs(alpha) : alpha;
	ASSERT(vec2_len == dim)

	SGVector<ST> vec = dense_feats->get_feature_vector(vec_idx1);

	int32_t hash_cache_size = use_quadratic ? vec.vlen : 0;
	SGVector<uint32_t> hash_cache(hash_cache_size);

	for (index_t i=0; i<vec.vlen; i++)
	{
		uint32_t h_idx = Hash::MurmurHash3((uint8_t* ) &i, sizeof (index_t), i);

		if (use_quadratic)
			hash_cache[i] = h_idx;

		if ( (!use_quadratic) || keep_linear_terms)
			vec2[h_idx % dim] += val * vec[i];
	}

	if (use_quadratic)
	{
		for (index_t i=0; i<vec.size(); i++)
		{
			int32_t n_idx = i * vec.size() + i;
			uint32_t idx = Hash::MurmurHash3((uint8_t* ) &n_idx, sizeof (index_t), n_idx) % dim;
			vec2[idx] += val * vec[i] * vec[i];

			for (index_t j=i+1; j<vec.size(); j++)
			{
				idx = (hash_cache[i] ^ hash_cache[j]) % dim;
				vec2[idx] += val * vec[i] * vec[j];
			}
		}
	}
	dense_feats->free_feature_vector(vec, vec_idx1);
}

template <class ST>
int32_t HashedDenseFeatures<ST>::get_nnz_features_for_vector(int32_t num) const
{
	return dim;
}

template <class ST>
void* HashedDenseFeatures<ST>::get_feature_iterator(int32_t vector_index)
{
	not_implemented(SOURCE_LOCATION);;
	return NULL;
}
template <class ST>
bool HashedDenseFeatures<ST>::get_next_feature(int32_t& index, float64_t& value,
	void* iterator)
{
	not_implemented(SOURCE_LOCATION);;
	return false;
}
template <class ST>
void HashedDenseFeatures<ST>::free_feature_iterator(void* iterator)
{
	not_implemented(SOURCE_LOCATION);;
}

template <class ST>
const char* HashedDenseFeatures<ST>::get_name() const
{
	return "HashedDenseFeatures";
}

template <class ST>
EFeatureType HashedDenseFeatures<ST>::get_feature_type() const
{
	return F_UINT;
}

template <class ST>
EFeatureClass HashedDenseFeatures<ST>::get_feature_class() const
{
	return C_SPARSE;
}

template <class ST>
int32_t HashedDenseFeatures<ST>::get_num_vectors() const
{
	return dense_feats->get_num_vectors();
}

template <class ST>
SGSparseVector<ST> HashedDenseFeatures<ST>::get_hashed_feature_vector(int32_t vec_idx) const
{
	SGVector<ST> vec = dense_feats->get_feature_vector(vec_idx);
	SGSparseVector<ST> hashed_vec = HashedDenseFeatures<ST>::hash_vector(
			vec, dim, use_quadratic, keep_linear_terms);
	dense_feats->free_feature_vector(vec, vec_idx);
	return hashed_vec;
}

template <class ST>
SGSparseVector<ST> HashedDenseFeatures<ST>::hash_vector(SGVector<ST> vec, int32_t dim,
	bool use_quadratic, bool keep_linear_terms)
{
	SGVector<ST> h_vec(dim);
	SGVector<ST>::fill_vector(h_vec.vector, dim, 0);

	int32_t hash_cache_size = use_quadratic ? vec.vlen : 0;
	SGVector<uint32_t> hash_cache(hash_cache_size);

	for (index_t i=0; i<vec.size(); i++)
	{
		uint32_t hash = Hash::MurmurHash3((uint8_t* ) &i, sizeof (index_t), i);
		if (use_quadratic)
			hash_cache[i] = hash;

		if ( (!use_quadratic) || keep_linear_terms)
			h_vec[hash % dim] += vec[i];
	}

	if (use_quadratic)
	{
		for (index_t i=0; i<vec.size(); i++)
		{
			index_t n_idx = i * vec.size() + i;
			uint32_t idx = Hash::MurmurHash3((uint8_t* ) &n_idx, sizeof (index_t), n_idx) % dim;
			h_vec[idx] += vec[i] * vec[i];

			for (index_t j=i+1; j<vec.size(); j++)
			{
				idx = (hash_cache[i] ^ hash_cache[j]) % dim;
				h_vec[idx] += vec[i] * vec[j];
			}
		}
	}

	int32_t num_nnz_feats = 0;
	for(index_t i=0; i<dim; i++)
	{
		if (h_vec[i]!=0)
			num_nnz_feats++;
	}

	SGSparseVector<ST> hashed_vector(num_nnz_feats);

	int32_t sparse_feat_index = 0;
	for (index_t i=0; i<dim; i++)
	{
		if (h_vec[i]!=0)
		{
			hashed_vector.features[sparse_feat_index].feat_index = i;
			hashed_vector.features[sparse_feat_index++].entry = h_vec[i];
		}
	}

	return hashed_vector;
}

template class HashedDenseFeatures<bool>;
template class HashedDenseFeatures<char>;
template class HashedDenseFeatures<int8_t>;
template class HashedDenseFeatures<uint8_t>;
template class HashedDenseFeatures<int16_t>;
template class HashedDenseFeatures<uint16_t>;
template class HashedDenseFeatures<int32_t>;
template class HashedDenseFeatures<uint32_t>;
template class HashedDenseFeatures<int64_t>;
template class HashedDenseFeatures<uint64_t>;
template class HashedDenseFeatures<float32_t>;
template class HashedDenseFeatures<float64_t>;
template class HashedDenseFeatures<floatmax_t>;
}
