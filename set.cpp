#include "set.h"

size_t case_insensitive::hash(const std::string &s)
{
	size_t sum = 0;
	size_t powr = 1;
	for (size_t i = 1; i <= s.size(); ++i)
	{
		powr = 1;
		for (size_t j = 0; j < s.size() - i; j++)
		{
			powr = powr * 31;
		}
		sum += tolower(s[i - 1]) * powr;
	}
	return sum;
}

bool case_insensitive::equal(const std::string &s1, const std::string &s2)
{
	if (s1.length() != s2.length())
	{
		return false;
	}

	for (size_t i = 0; i < s1.length(); ++i)
	{
		if (tolower(s1[i]) != tolower(s2[i]))
		{
			return false;
		}
	}
	return true;
}

bool set::insert_norehash(const std::string &s)
{
	size_t hash_index = case_insensitive::hash(s) % buckets.size();

	for (const auto &str : buckets[hash_index])
	{
		if (case_insensitive::equal(str, s))
		{
			return false;
		}
	}

	buckets[hash_index].push_back(s);
	set_size++;
	return true;
}

void set::consider_rehash()
{
	if (loadfactor() > max_load_factor)
	{
		size_t i = 0;
		size_t buckets_required = 0;
		double cur_loadfactor = loadfactor();

		while (cur_loadfactor > max_load_factor)
		{
			buckets_required = 8 << i;
			cur_loadfactor = static_cast<double>(set_size) / buckets_required;
			++i;
		}
		// finds the new number of buckets that allow loadfactor to be less than max

		std::vector<buckettype> new_buckets(buckets_required);
		// creating new vector with the appropriate number of buckets

		for (size_t i = 0; i < buckets.size(); ++i)
		{
			for (auto it = buckets[i].begin(); it != buckets[i].end(); ++it)
			{
				size_t hash_new_index = case_insensitive::hash(*it) % new_buckets.size();
				new_buckets[hash_new_index].push_back(*it);
			}
		}
		// rehashing all elements from the old buckets

		buckets = new_buckets;
	}
}

set::set(std::initializer_list<std::string> init, double max_load_factor) : set_size(0),
																			max_load_factor(max_load_factor),
																			buckets(std::vector<buckettype>(8))
{
	if (max_load_factor <= 0.0)
	{
		throw std::runtime_error("max load factor must be > 0");
	}
	for (std::string s : init)
	{
		insert(s);
	}
}

bool set::contains(const std::string &s) const
{
	size_t hash_index = case_insensitive::hash(s) % buckets.size();

	for (const auto &str : buckets[hash_index])
	{
		if (case_insensitive::equal(str, s))
		{
			return true;
		}
	}
	return false;
}

bool set::insert(const std::string &s)
{
	if (insert_norehash(s))
	{
		consider_rehash();
		return true;
	};
	return false;
}

bool set::remove(const std::string &s)
{
	size_t hash_index = case_insensitive::hash(s) % buckets.size();

	for (auto it = buckets[hash_index].begin(); it != buckets[hash_index].end(); ++it)
	{
		if (case_insensitive::equal(*it, s))
		{
			buckets[hash_index].erase(it);
			set_size--;
			return true;
		}
	}
	return false;
}

size_t set::size() const
{
	return set_size;
}

bool set::empty() const
{
	if (set_size == 0)
	{
		return true;
	}
	return false;
}

void set::clear()
{
	for (size_t i = 0; i < buckets.size(); ++i)
	{
		buckets[i].clear();
	}
	set_size = 0;
}

void set::print(std::ostream &out) const
{
	for (size_t i = 0; i < buckets.size(); ++i)
	{
		out << "bucket[" << i << "]: { ";
		for (auto it = buckets[i].begin(); it != buckets[i].end(); ++it)
		{
			if (*it == buckets[i].back())
			{
				out << *it;
			}
			else
			{
				out << *it << ", ";
			}
		}
		out << " }\n";
	}
}
