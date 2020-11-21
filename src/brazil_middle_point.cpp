#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <numeric>

using arr_type = std::array<double, 3>;

struct acc {
   auto operator()(double init, arr_type const& value) const
   {
      init += value.at(0);
      return init;
   };
};

struct sort_cond {
   int i = 1;
   auto operator()(arr_type const& u, arr_type const& v) const
   {
      return u.at(i) < v.at(i);
   };
};

int acc_up_to(std::vector<arr_type> const& v, double area)
{
   auto d = 0.0;
   auto i = 0;

   while (d < area)
      d += v.at(++i).at(0);

   return i;
}

auto find_partitions(std::vector<arr_type> v)
{
   auto const total_area =
      std::accumulate( std::cbegin(v)
                     , std::cend(v)
                     , 0.0
                     , acc{});

   auto const half = total_area / 2;

   std::sort(std::begin(v), std::end(v), sort_cond{1});
   auto const i1 = acc_up_to(v, half);
   auto const v1 = v.at(i1).at(1);

   std::sort(std::begin(v), std::end(v), sort_cond{2});
   auto const i2 = acc_up_to(v, half);
   auto const v2 = v.at(i2).at(2);

   return std::make_pair(v1, v2);
}

std::vector<std::string>
split_line(std::string const& line, char sep)
{
   std::string item;
   std::istringstream iss(line);
   std::vector<std::string> fields;
   while (std::getline(iss, item, sep))
      fields.push_back(item);

   return fields;
}

auto central_point(std::string const& str)
{
   std::istringstream iss(str);

   std::vector<arr_type> table;
   std::string line;
   while (std::getline(iss, line)) {
      auto fields = split_line(line, ':');

      if (std::size(fields) != 4) {
         std::cerr << "Incompatible line size on line: ";
	 std::copy( std::cbegin(fields)
                  , std::cend(fields)
                  , std::ostream_iterator<std::string>(std::cerr, " "));
	 std::cerr << std::endl;
         return std::pair<double, double>{0.0, 0.0};
      }

      table.push_back({ std::stod(fields.at(1))
                      , std::stod(fields.at(2))
                      , std::stod(fields.at(3))});
   }

   return find_partitions(table);
}

int main()
{
   using iter_type = std::istreambuf_iterator<char>;

   std::ifstream ifs {"merge.txt"};
   std::string str {iter_type {ifs}, {}};

   auto const out = central_point(str);

   std::cout << out.first << " " << out.second << std::endl;
}
