/* Create Table */
/* Primary key: id */
/* foreign key: dept_name <- department */
create instructor(
	id char(5),
	name varchar(20) not null,
	dept_name varchar(20),
	salary numeric(8, 2),
	primary key(id), 
	foreign key (dept_name) references department);

/* Add Attribute */
alter table instructor add sex char(1)  default 1;

/* Delete Attribute */
alter table instructor drop salary;

/*************************DELETE*********************/
delete from instructor;
where dept_name='Finance';

/*Watson에 있는 instructor 삭제*/
delete from instructor
where dept_name in (select dept_name
				   from department
				   where building='Watson');
				   
/* 평균 instructor보다 돈 못받는 애들 다삭제 avg값 변경 x 미리 계산해놓음 */
delete from instructor
where salary > (select avg(salary)
			   from instructor);
			   
insert into course(course_id, title, dept_name, credits)/*순서가정*/
values ('CS-437', 'Database Systems', 'Comp. Sci.' null); /*null값 가느 */ 

/* 모든 교수 student에 넣음 */
insert into student
select id, name, dept_name, 0
from instructor;
			   
/* 100000이 넘으면 3퍼센트 올려줌 */
update instructor
set salary = salary * 1.03;
where salary > 100000;
update instructor
set salary = salary * 1.05;
where salary <= 100000;

/*case 써라 */
update instructor
 set salary = case when salary <= 100000 then salary * 1.05 else salary * 1.03 
 end;
	

/* 이수한 학점가지고 학생들의 tot_cred 변경 */
update student as A
set tot_cred = (select sum(credits)
			   from takes natural join course	
			   where student.id = takes.id and grade <> 'F' and takes.grade is	 not null);
			   
update student as A
set tot_cred = (select case when sum(credits) is null then 0 else sum(credits) end
			   from takes natural join course
			   where student.id =takes.id and takes.grade <> 'F' and takes.grade is not null)

/* viewwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww*/

create view faculty as 
(select id, name, dept_name
from instructor);

create view department_total_salary(dept_name, total_salary) as
	select dept_name, sum(salary)
	from instructor
	group by dept_name;


/* view 중첩 사용 가능 */

create view physics_fall_2009 as
select course_id, sec_id, building, room_number
from course natural join section
where dept_name='Physics' and semester='Fall' and year='2009';


select *
from physics_fall_2009;

/* 없는 곳은 null로 채워짐 primary key 제외 */
insert into faculty values('30765', 'Green', 'Music');

/* insert는 view의 from에 하나의 table만 있어야함
select에 무조건 순수 attribute만 잇어야 함 */
/* attribute가 없으면 null로 set될 수 있어야 함*/
/* group by and having 없어야 함 */

create view history_instructor as
select *
from instructor
where dept_name ='History'
with check option;

insert into history_instructor values('25566', 'Brown', 'Biology', 100000);


/*Transaction, begine 부터 end까지 실행, 이 명령어가 모두 정상적으로 실행되지 않으면, rollback으로 이전으로 돌아갈 수 있음*/
begin;
delete from instructor
where salary < 1000; 

end;

create table section(
	course_id varchar(8),
	sec_id varchar(8),
	semester varchar(6),
	year numeric(4,0),
	building varchar(15),
	room_number varcher(7),
	time_slot_id varchar(4),
	primary key(course_id, sec_id, semester, year),
	check(semester in ('Spring', 'Summer', 'Fall', 'Winter')) /*Attribute의 값을 지정할 수 있음*/
);

create table course2(
	course_id char(5) primary key,
	title varchar(20),
	/* dept_name varchar(20) references department */
	dept_name varchar(20),
	foreign key(dept_name) references department
		on delete set null
		on update cascade,
);

create table person(
	id char(10),
	name char(40),
	mother char(10) not null, 
	father char(10) not null,
	primary key (id),
	foreign key (father) references person,
	foreign key (mother) references person
);

insert into person values('12', 'dasd', '1', '2'); 
insert into person values('1', 'dasd', '3', '1'); 
insert into person values('2', 'dasd', '4', '5'); 
/* 이럼 not null이라 처리도 안되고, father mother이 없어서 아예 insert가 안됨 */

create table person(
	id char(10) primary key,
	name char(40) default 'unkwown',
	mother char(10) not null, 
	father char(10) not null,
	constraint father_ref foreign key (father) references person,
	constraint mother_ref foreign key (mother) references person
);

set constraints father_ref, mother_ref deferred; 
/*foreign key가 들어가기 전에, 한 번 유예하여 삽입 가능 */


create index Father on person(father); 
