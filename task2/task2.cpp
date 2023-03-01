#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <exception>


struct Query
{
	std::string name;
	std::vector<std::string> column;
	std::string from;
	std::map<std::string, std::string> where;
};

class SqlSelectQueryBuilder
{
private:
	Query query;
	std::string getColumn()
	{
		std::string outColumn = "";
		if (!query.column.empty())
		{
			size_t index = 1;
			for (auto itr = query.column.begin(); itr != query.column.end(); ++itr)
			{
				outColumn += *itr;
				if (index < query.column.size())
				{
					outColumn = outColumn + ", ";
					++index;
				}
			}
		}
		else
			outColumn = "*";
		return outColumn;
	}
	std::string getFrom()
	{
		std::string outFrom = "";
		if (!query.from.empty())
		{
			outFrom = "FROM " + query.from;
		}
		return outFrom;
	}
	std::string getWhere()
	{
		std::string outWhere = "";
		if (!query.where.empty())
		{
			size_t index = 1;
			for (auto itr = query.where.begin(); itr != query.where.end(); ++itr)
			{
				outWhere = outWhere + itr->first + "=" + itr->second;
				if (index < query.where.size())
				{
					outWhere = outWhere + " AND ";
					++index;
				}
			}
			outWhere = "WHERE " + outWhere;
		}
		return outWhere;
	}
	void checkQuery()
	{
		if (query.column.empty() && query.where.empty() && query.from.empty())
			throw std::runtime_error("Query is empty!");
	}
public:
	SqlSelectQueryBuilder()
	{
		query.name = "SELECT";
	}
	SqlSelectQueryBuilder& AddColumn(const std::string&& columnName)
	{
		query.column.push_back(columnName);
		return *this;
	}
	SqlSelectQueryBuilder& AddColumn(const std::vector<std::string>& columns) noexcept
	{
		for (const auto el : columns)
			query.column.push_back(el);
		return *this;
	}
	SqlSelectQueryBuilder& AddFrom(const std::string&& from)
	{
		query.from = from;
		return *this;
	}
	SqlSelectQueryBuilder& AddWhere(const std::string&& key, const std::string&& val)
	{
		query.where.insert(std::pair<std::string, std::string>(key, val));
		return *this;
	}
	SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept
	{
		for (auto itr = kv.begin(); itr != kv.end(); ++itr)
			query.where.insert(std::pair<std::string, std::string>(itr->first, itr->second));
		return *this;
	}
	std::string BuildQuery()
	{
		checkQuery();
		std::string outQuery = "\"" + query.name + " " + getColumn() + " " + getFrom() + " " + getWhere() + ";\"";
		return outQuery;
	}
};

int main(int argc, char** argv)
{
	try
	{
		SqlSelectQueryBuilder queryBuilder;
		queryBuilder.AddColumn("name").AddColumn("phone");
		queryBuilder.AddFrom("students");
		queryBuilder.AddWhere("id", "42").AddWhere("name", "John");

		std::vector<std::string> col;
		col.push_back("n");
		col.push_back("p");
		queryBuilder.AddColumn(col);

		std::map<std::string, std::string> wh{ {"i", "4"}, {"n", "J"} };
		queryBuilder.AddWhere(wh);

		std::cout << queryBuilder.BuildQuery();
	}
	catch (std::runtime_error& ex) { std::cout << ex.what() << std::endl; }

	return 0;
}