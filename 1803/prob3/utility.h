#include <boost/multiprecision/cpp_int.hpp>
#include <vector>
#include <valarray>
#include <cinttypes>
#include <stdexcept>
#include <type_traits>

//using namespace boost::multiprecision;

bool is_palindrome(const boost::multiprecision::uint256_t& value);
std::vector<boost::multiprecision::uint256_t> prime_array(size_t nth);
std::vector<boost::multiprecision::uint256_t> prime_product_array(size_t nth);
std::vector<boost::multiprecision::uint256_t> prime_product_divisors(std::uint8_t nth);


template<class T>
inline constexpr T pow_int(const T base, unsigned const exponent)
{
    // (parentheses not required in next line)
    return (exponent == 0)     ? 1 :
    (exponent % 2 == 0) ? pow_int(base, exponent/2)*pow_int(base, exponent/2) :
    base * pow_int(base, (exponent-1)/2) * pow_int(base, (exponent-1)/2);
}

template <class Class, size_t Size>
struct memory_request{
    static void dynamic_assert(){
        unsigned long long phypz = sysconf(_SC_PHYS_PAGES);
        unsigned long long psize = sysconf(_SC_PAGE_SIZE);
        if ((phypz * psize - 2147483648ull) < sizeof(Class)*Size)
            throw std::overflow_error("memory overflow.");
        else if ((phypz * psize - 4294967296ull) < sizeof(Class)*Size){
            std::clog<< sizeof(Class)*Size/1048576<<"MB / " << phypz * psize/1048576 << "MB" << std::endl;
        }
    }
};

template < typename T, T base, size_t exponent >
struct pow_traits;
template < typename T, T base, size_t exponent >
struct pow_traits: std::integral_constant < T, pow_traits<T,base,exponent-1>::value >{};
template < typename T, T base >
struct pow_traits<T,base,0>:std::integral_constant < T, 1 >{};

template <class Class>
void memory_request_dynamic_assert(size_t Size){
    unsigned long long phypz = sysconf(_SC_PHYS_PAGES);
    unsigned long long psize = sysconf(_SC_PAGE_SIZE);
    if ((phypz * psize - 2147483648ull) < sizeof(Class)*Size)
        throw std::overflow_error("memory overflow.");
    else if ((phypz * psize - 4294967296ull) < sizeof(Class)*Size){
        std::clog<< sizeof(Class)*Size/1048576<<"MB / " << phypz * psize/1048576 << "MB" << std::endl;
    }
}

/*
template <typename Integer = boost::multiprecision::int256_t>
std::vector<Integer> prime_array(size_t nth) {
	std::vector<Integer> result{ 1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, \
		61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, \
		137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, \
		211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, \
		283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, \
		379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, \
		461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, \
		563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, \
		643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, \
		739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, \
		829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, \
		937, 941, 947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013, 1019, \
		1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069, 1087, 1091, \
		1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151, 1153, 1163, 1171, \
		1181, 1187, 1193, 1201, 1213, 1217, 1223, 1229, 1231, 1237, 1249, \
		1259, 1277, 1279, 1283, 1289, 1291, 1297, 1301, 1303, 1307, 1319, \
		1321, 1327, 1361, 1367, 1373, 1381, 1399, 1409, 1423, 1427, 1429, \
		1433, 1439, 1447, 1451, 1453, 1459, 1471, 1481, 1483, 1487, 1489, \
		1493, 1499, 1511, 1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, \
		1579, 1583, 1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, \
		1657, 1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733, \
		1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811, 1823, \
		1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889, 1901, 1907, \
		1913, 1931, 1933, 1949, 1951, 1973, 1979 };
	if (nth < 300) {
		result.resize(nth + 1);
		return result;
	}
	result.reserve(nth + 17);
	std::valarray<int> sieveval{ 1, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 49, 53, 59 };
	std::valarray<bool> sieve(false, 16);
	sieveval += 1980;
    for (unsigned long max_val = 4000; max_val < std::numeric_limits<Integer>::max()-2000; max_val += 2000) {
		size_t max_prime_range = std::sqrt(max_val)*(1-3e-16);
		while (sieveval[15] < max_val) {
			sieve = true;
			for (size_t i = 0; i < 16; ++i) {
				for (size_t j = 4; result[j] < max_prime_range; ++j) {
					if (sieveval[i] % result[j] == 0) {
						sieve[i] = false;
						break;
					}
				}
				if (sieve[i]) result.push_back(sieveval[i]);
			}
			sieve += 60;
			if (nth <= result.size()) {
				result.resize(nth + 1);
				return result;
			}
		}
	}
	result.resize(nth + 1);
	return result;
}

template <typename Integer>
Integer prime(Integer nth) {
	return prime_array(nth).back();
}

template <typename Integer = boost::multiprecision::uint256_t>
std::vector<Integer> prime_product_array(size_t nth) {
	auto primes = prime_array<Integer>(nth);
	std::vector<Integer> result(nth + 1);
	result[0] = 1;
	for (size_t i = 1; i <= nth; ++i) 
		result[i] = result[i - 1] * primes[i];
	return result;
}

template <typename Integer, typename MaskType=Integer>
std::vector<Integer> prime_product_divisors(MaskType nth) {
	auto primes = prime_array<Integer>(nth);
	std::vector<Integer> divisors;
	for (MaskType i = 0; i < (1ull << nth); ++i) {
		Integer temp = i;
		Integer result = 1;
		for (MaskType j = 0; j < nth; ++j) {
			if (temp & 1) result *= primes[j + 1];
			temp >>= 1;
		}
		divisors.push_back(result);
	}
	std::sort(divisors.begin(), divisors.end());
	return divisors;
}*/
