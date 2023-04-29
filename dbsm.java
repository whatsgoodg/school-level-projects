import java.sql.*;
import java.lang.*;
import java.util.ArrayList;

public class Main {
    public static Connection conn;

    public static void main(String[] args) throws Exception {

        try {
            Class.forName("org.postgresql.Driver");
        } catch (ClassNotFoundException e) {
            System.out.println("Where is your PostgreSQL JDBC Driver? Include in your library path!");
            e.printStackTrace();
            return;
        }
        System.out.println("PostgreSQL JDBC Driver Registered!");
        /// if you have a error in this part, check jdbc driver(.jar file)

        conn = null;

        try {
            conn = DriverManager.getConnection(
                    "jdbc:postgresql://127.0.0.1:5432/project_movie", "postgres", "cse3207");
        } catch (SQLException e) {
            System.out.println("Connection Failed! Check output console");
            e.printStackTrace();
            return;
        }
        /// if you have a error in this part, check DB information (db_name, user name, password)

        if (conn != null) {
            System.out.println(conn);
            System.out.println("You made it, take control your database now!");
        } else {
            System.out.println("Failed to make connection!");
        }

        create_tables();
        insert_intials1();
        insert_intials2();
        //delete_tables();
        conn.close();
    }
    public static void create_tables() {
        String[] tables = new String[13];
        String director = "create table director(";
        director += "\"directorID\" INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,";
        director += "\"directorName\" VARCHAR(25),";
        director += "\"dateOfBirth\" CHAR(10),";
        director += "\"dateOfDeath\" CHAR(10));";
        tables[0] = director;

        String actor = "create table actor(";
        actor += "\"actorID\" INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,";
        actor += "\"actorName\" VARCHAR(25),";
        actor += "\"dateOfBirth\" CHAR(10),";
        actor += "\"dateOfDeath\" CHAR(10),";
        actor += "gender VARCHAR(6));";
        tables[1] = actor;

        String movie = "create table movie(";
        movie += "\"movieID\" INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,";
        movie += "\"movieName\" VARCHAR(25) not null,";
        movie += "\"releaseYear\" CHAR(4) not null,";
        movie += "\"releaseMonth\" VARCHAR(8) not null,";
        movie += "\"releaseDate\" CHAR(10) not null,";
        movie += "\"publisherName\" VARCHAR(30) not null,";
        movie += "\"avgRate\" numeric(1, 1) check (\"avgRate\" >= 0));";
        tables[2] = movie;

        String award = "create table award(";
        award += "\"awardID\" INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,";
        award += "\"awardName\" VARCHAR(25));";
        tables[3] = award;

        String genre = "create table genre(";
        genre += "\"genreName\" VARCHAR(10) PRIMARY KEY);";
        tables[4] = genre;

        String movieGenre= "create table \"movieGenre\"(";
        movieGenre += "\"movieID\" integer,";
        movieGenre += "\"genreName\" VARCHAR(10),";
        movieGenre += "PRIMARY KEY(\"movieID\", \"genreName\"),";
        movieGenre += "FOREIGN KEY(\"movieID\") references movie ";
        movieGenre += "on delete cascade,";
        movieGenre += "FOREIGN KEY(\"genreName\") references genre ";
        movieGenre += "on delete cascade);";
        tables[5] = movieGenre;

        String movieObtain= "create table \"movieObtain\"(";
        movieObtain += "\"movieID\" integer,";
        movieObtain += "\"awardID\" integer,";
        movieObtain += "\"year\" CHAR(4) not null,";
        movieObtain += "PRIMARY KEY(\"movieID\", \"awardID\"),";
        movieObtain += "FOREIGN KEY(\"movieID\") references movie ";
        movieObtain += "on delete cascade,";
        movieObtain += "FOREIGN KEY(\"awardID\") references award ";
        movieObtain += "on delete cascade);";
        tables[6] = movieObtain;

        String actorObtain= "create table \"actorObtain\"(";
        actorObtain += "\"actorID\" integer,";
        actorObtain += "\"awardID\" integer,";
        actorObtain += "\"year\" CHAR(4) not null,";
        actorObtain += "PRIMARY KEY(\"actorID\", \"awardID\"),";
        actorObtain += "FOREIGN KEY(\"actorID\") references actor ";
        actorObtain += "on delete cascade,";
        actorObtain += "FOREIGN KEY(\"awardID\") references award ";
        actorObtain += "on delete cascade);";
        tables[7] = actorObtain;

        String directorObtain= "create table \"directorObtain\"(";
        directorObtain += "\"directorID\" integer,";
        directorObtain += "\"awardID\" integer,";
        directorObtain += "\"year\" CHAR(4) not null,";
        directorObtain += "PRIMARY KEY(\"directorID\", \"awardID\"),";
        directorObtain += "FOREIGN KEY(\"directorID\") references director ";
        directorObtain += "on delete cascade,";
        directorObtain += "FOREIGN KEY(\"awardID\") references award ";
        directorObtain += "on delete cascade);";
        tables[8] = directorObtain;

        String cast= "create table \"cast\"(";
        cast += "\"movieID\" integer,";
        cast += "\"actorID\" integer,";
        cast += "role VARCHAR(16),  ";
        cast += "PRIMARY KEY(\"movieID\", \"actorID\"),";
        cast += "FOREIGN KEY(\"movieID\") references movie ";
        cast += "on delete cascade,";
        cast += "FOREIGN KEY(\"actorID\") references actor ";
        cast += "on delete cascade);";
        tables[9] = cast;

        String make= "create table \"make\"(";
        make += "\"movieID\" integer,";
        make += "\"directorID\" integer,";
        make += "PRIMARY KEY(\"movieID\", \"directorID\"),";
        make += "FOREIGN KEY(\"movieID\") references movie ";
        make += "on delete cascade,";
        make += "FOREIGN KEY(\"directorID\") references director ";
        make += "on delete cascade);";
        tables[10] = make;

        String customer = "create table customer(";
        customer += "\"customerID\" INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,";
        customer += "\"customerName\" VARCHAR(25) not null,";
        customer += "\"dateOfBirth\" CHAR(10) not null,";
        customer += "gender VARCHAR(6) not null);";
        tables[11] = customer;

        String customerRate= "create table \"customerRate\"(";
        customerRate += "\"customerID\" integer,";
        customerRate += "\"movieID\" integer,";
        customerRate += "\"customerRate\" integer not null,";
        customerRate += "PRIMARY KEY(\"customerID\", \"movieID\"),";
        customerRate += "FOREIGN KEY(\"customerID\") references customer ";
        customerRate += "on delete cascade,";
        customerRate += "FOREIGN KEY(\"movieID\") references movie ";
        customerRate += "on delete cascade);";
        tables[12] = customerRate;

        try {
            Statement stmt = conn.createStatement();
            for(int i =0; i < 13; i++){
                System.out.println(tables[i]);
                stmt.executeUpdate(tables[i]);
            }
        } catch (SQLException e) {
            System.out.println("Table Creation failed! Check output console");
            e.printStackTrace();
            return;
        }

    }

    public static void insert_intials1(){
        String actors[] = {
                "\'Marlon Brando\', \'1924.04.03\', \'2004.07.01\', \'Male\'",
                "\'Robert Duvall\', \'1931.01.05\', null, \'Male\'",
                "\'Al Pacino\', \'1940.04.25\', null, \'Male\'",
                "\'Sam Worthington\', \'1976.08.02\', null, \'Male\'",
                "\'Zoe Saldana\', \'1978.06.19\', null, \'Female\'",
                "\'Leonardo DiCaprio\', \'1974.11.11\', null, \'Male\'",
                "\'Kate Winslet\', \'1975.10.05\', null, \'Female\'",
                "\'Fionn Whiteheadt\', \'1997.07.18\', null, \'Male\'",
                "\'Tom Hardy\', \'1977.09.15\', null, \'Male\'"
        };
        String customers[] = {
                "\'Bob\', \'1996.08.21\', \'Male\'",
                "\'John\', \'1989.06.23\', \'Male\'",
                "\'Jack\', \'1970.05.04\', \'Male\'",
                "\'Jill\', \'1987.04.27\', \'Female\'" ,
                "\'Bell\', \'2001.12.14\', \'Female\'"
        };

        String directors[] ={
                "\'Francis Ford Coppola\', \'1939.04.07\', null",
                "\'James Cameron\', \'1954.08.16\', null",
                "\'Christopher Nolan|\', \'1970.07.30\', null"
        };

        try {
            Statement stmt = conn.createStatement();
            for(int i =0; i < 9; i++) {
                String insert_actor = "insert into actor (\"actorName\", \"dateOfBirth\", \"dateOfDeath\", gender) ";
                insert_actor += "values (" + actors[i] + ");";
                System.out.println(insert_actor);
                stmt.executeUpdate(insert_actor);
            }

            for(int i =0; i < 5; i++){
                String insert_customer = "insert into customer (\"customerName\", \"dateOfBirth\", gender) ";
                insert_customer += "values (" + customers[i] + ");";
                stmt.executeUpdate(insert_customer);
            }

            for(int i = 0; i < 3; i++){
                String insert_director = "insert into director (\"directorName\", \"dateOfBirth\", \"dateOfDeath\") ";
                insert_director += "values (" + directors[i] + ");";
                System.out.println(insert_director);
                stmt.executeUpdate(insert_director);
            }
        } catch (SQLException e) {
            System.out.println("Initial data insertion failed! Check output console");
            e.printStackTrace();
            return;
        }


    }
    static void insert_intials2(){
        String movies[] ={
                "\'Apocalypse Now\', \'1979\', \'May\', \'1979.05.19\', \'Omni Zoetrope\'",
                "\'The Godfather\', \'1972\', \'March\', \'1972.03.14\', \'Paramount Pictures\'",
                "\'Avatar\', \'2009\', \'December\', \'2009.12.10\', \'20th Century Fox\'",
                "\'Titanic\', \'1997\', \'November\', \'1997.11.01\', \'Paramount Pictures\'",
                "\'Dunkirk\', \'2017\', \'July\', \'2017.07.13\', \'Warner Bros. Pictures\'",
                "\'Inception\', \'2010\', \'July\', \'2010.07.08\', \'Warner Bros. Pictures\'"
        };

        String casts[][][] = {
                {{"1", "\'Main actor\'"}, {"2", "\'Supporting actor\'"}},
                {{"1", "\'Main actor\'"}, {"3", "\'Main actor\'"}},
                {{"4", "\'Main actor\'"}, {"5", "\'Main actor\'"}},
                {{"6", "\'Main actor\'"}, {"7", "\'Main actor\'"}},
                {{"8", "\'Main actor\'"}, {"9", "\'Main actor\'"}},
                {{"6", "\'Main actor\'"}},
        };

        String makes[] = {
                "1", "1", "2", "2", "3", "3"
        };

        String genres[][] ={
                { "\'Drama\'", "\'Mystery\'","\'War\'"},
                {"\'Crime\'", "\'Drama\'"},
                {"\'Action\'", "\'Adventure\'", "\'Fantasy\'"},
                {"\'Drama\'", "\'Romance\'"},
                {"\'Action\'", "\'Drama\'", "\'Thriller\'", "\'War\'"},
                {"\'Action\'", "\'Adventure\'", "\'Sci-Fi\'"}
        };

        try {
            Statement stmt = conn.createStatement();
            for(int i =0; i < 6; i++) {
                String insert_movie = "insert into movie (\"movieName\", \"releaseYear\", \"releaseMonth\", ";
                insert_movie += "\"releaseDate\", \"publisherName\", \"avgRate\") ";
                insert_movie += "values (" + movies[i] + ", " + Integer.toString(0) + ");";

                String insert_make = "insert into make (\"movieID\", \"directorID\") ";
                insert_make += "values (" + Integer.toString(i + 1) + ", " + makes[i] + ");";
                stmt.executeUpdate(insert_movie);
                stmt.executeUpdate(insert_make);

                for(int j = 0; j < casts[i].length; j++){
                    String insert_cast = "insert into \"cast\" (\"movieID\", \"actorID\", \"role\") ";
                    insert_cast += "values (" +  Integer.toString(i + 1) + ", " + casts[i][j][0] + ", ";
                    insert_cast += casts[i][j][1] + ");";
                    System.out.println(insert_cast);
                    stmt.executeUpdate(insert_cast);
                }


                for(int j = 0; j < genres[i].length; j++) {
                    String insert_genre = "insert into genre ";
                    insert_genre += "values (" + genres[i][j] + ") ";
                    insert_genre += "ON CONFLICT (\"genreName\") DO NOTHING;";
                    System.out.println(insert_genre);
                    stmt.executeUpdate(insert_genre);
                    insert_genre = "insert into \"movieGenre\" ";
                    insert_genre += "values (" + Integer.toString(i + 1) + ", " + genres[i][j] + ");";
                    System.out.println(insert_genre);
                    stmt.executeUpdate(insert_genre);
                }
            }
        } catch (SQLException e) {
            System.out.println("Initial data insertion failed! Check output console");
            e.printStackTrace();
            return;
        }
    }
    static void delete_tables(){
        String[] tables = {"\"movieGenre\"", "\"movieObtain\"", "\"actorObtain\"",
                "\"directorObtain\"", "\"cast\"",  "\"customerRate\"", "make", "director", "actor",
                "movie", "award", "genre", "customer"};

        try {
            Statement stmt = conn.createStatement();
            for(int i =0; i < 13; i++) {
                String delete_query = "drop table ";
                delete_query += tables[i];
                stmt.executeUpdate(delete_query);
            }
        } catch (SQLException e) {
            System.out.println("Table deletion failed! Check output console");
            e.printStackTrace();
            return;
        }
    }
}
