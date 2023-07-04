#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <exception>
#include <memory>

class MyException :public std::exception{
private:
    std::string msg;
public:
    explicit MyException(std::string  message):msg(std::move(message)){}
    explicit MyException(const char* message): msg(message) {}
    virtual ~MyException() noexcept {}
    [[nodiscard]] const char* what() const noexcept override {
        return msg.c_str();
    }

};

class SqlSelectQuery{
public:
    SqlSelectQuery() = default;
    virtual ~SqlSelectQuery() = default;
    SqlSelectQuery(const SqlSelectQuery& other) = delete;
    SqlSelectQuery &operator= (const SqlSelectQuery &other) = delete;
    std::string query;
    std::vector<std::string> column;
    std::string table;
    std::vector<std::pair<std::string,std::string>> where;
    std::vector<std::string> operators;
};

class SqlSelectQueryBuilder{
public:
    SqlSelectQueryBuilder(){
        selectQuery = std::make_unique<SqlSelectQuery>();
    }
    virtual ~SqlSelectQueryBuilder() = default;
    SqlSelectQueryBuilder(const SqlSelectQueryBuilder &other) = delete;
    SqlSelectQueryBuilder &operator= (const SqlSelectQueryBuilder &other) = delete;

    static SqlSelectQueryBuilder creat(){ return {};}
    SqlSelectQueryBuilder &ClearAllInfo(){
        selectQuery->query.clear();
        selectQuery->column.clear();
        selectQuery->where.clear();
        selectQuery->operators.clear();
        selectQuery->table.clear();
        return *this;
    }
    SqlSelectQueryBuilder &AddColumn (const std::string &column){
        selectQuery->column.push_back(column);
        return *this;
    }
    SqlSelectQueryBuilder &AddFrom (const std::string &table){
        selectQuery->table = table;
        return *this;
    }
    SqlSelectQueryBuilder &AddWhere (const std::string &one, const std::string &two){
        selectQuery->where.emplace_back(one, two);
        return *this;
    }
    SqlSelectQueryBuilder &AddOperators(const std::string &operators){
        selectQuery->operators.push_back(operators);
        return *this;
    }
    [[nodiscard]] std::string BuildQuery(){
        selectQuery->query = "Select ";
        add_column();
        add_table();
        add_where();
        return selectQuery->query;
        }
private:
    void add_column(){
        if(selectQuery->column.empty()){
            selectQuery->query += "* ";
        }else{
            for(std::vector<std::string>::size_type i = 0; i < selectQuery->column.size(); ++i){
                if(i > 0){
                    selectQuery->query[selectQuery->query.size() - 1] = ',';
                    selectQuery->query += ' ';
                }
                selectQuery->query += selectQuery->column[i];
                selectQuery->query += ' ';
            }
        }
    }
    void add_table(){
        if(selectQuery->table.empty()){
            throw MyException("Table name not added");
        }
        selectQuery->query += "FROM " + selectQuery->table;
    }
    virtual void add_where(){
        selectQuery->query += " WHERE ";
        if((selectQuery->where.size() - 1) != selectQuery->operators.size()){
            throw MyException("The number of conditions does not match the number of operators");
        }
        for(std::vector<std::string>::size_type i = 0; i < selectQuery->where.size(); ++i){
            if(i > 0){
                selectQuery->query += ' ' + selectQuery->operators[i-1] + ' ';
            }
            selectQuery->query+= selectQuery->where[i].first + " = " + selectQuery->where[i].second;

        }
    }
    std::unique_ptr<SqlSelectQuery> selectQuery;
};

int main() {
    try {
    SqlSelectQueryBuilder sql;
    sql.AddColumn("name").AddFrom("table").AddWhere("name", "alex")
    .AddOperators("OR").AddWhere("name", "pax");
    std::cout << sql.BuildQuery();
    SqlSelectQueryBuilder sql1;
    }catch (const MyException &e){
        std::cout << e.what() << std::endl;
    }catch (const std::exception &e){
        std::cout << e.what() << std::endl;
    }
    return 0;
}
