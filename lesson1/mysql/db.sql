drop database if exists lesson;
create database lesson;
use lesson;

drop table if exists Linux;
create table Linux(
  Lid int not null primary key auto_increment,
  Lname varchar(50),
  content text
);

drop table if exists C;
create table C(
  Cid int not null primary key auto_increment,
  Cname varchar(50)
);

drop table if exists users;
create table users(
  id int not null primary key auto_increment,
  name varchar(20) comment '用户名',
  password char(32)
);
