# mgcdb
An embeddable in-memory database

## Create a table
A new table is created using the 'create' keyword, specifying the table name and a comma separated list of column names for the table.
Columns are untyped with _all_ data being stored as strings. columns with numerical data are implicitly converted when performing comparisons. 
(this is very likely to change)

     create table employees (name, position, age, salary, phone_number);


## Add data to a table
A table can be populated using the 'insert' keyword, specifiying the table to populate, a comma separated list of the columns to populate, and a comma separated
list of the values for those columns. If the table you are trying to populate has not yet been created, it will automatically create the table with the specified name. 
Likewise, if you specify a column that is not present in the table, it will add the desired column to the table.

     insert into employees (name, position, age, salary, phone_number) values (max, developer, 37, 1000000, 555-555-5555);

## Query data from table
Querying data with the 'select' keyword allows you to specify which columns (or all of them using the '*' signifier) from the specified table you would like to retrieve.
results can be filtered with the addition of a where clause, as well as sorted by a specified column:

      select * from employees order by salary;
      select name, age from employees where salary > 100000;
      select position, age from employees where salary > 150000 order by position

## Update data in table
Data in the table can also be updated

      update employees set position = ceo where name = max;

## Delete data from table
And of course, data can be deleted from the table using the 'delete' keyword. The where clause is used to specify _which_ records you would like to delete.

      delete from employees where salary > 200000;



