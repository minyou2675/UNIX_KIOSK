#include <gtk/gtk.h>
#include <sqlite3.h>
#include <stdio.h>
//gcc -o gtk gtk.c `pkg-config --libs --clfags gtk+-2.0`
int callback(int, char **, char **);
enum{
 LIST_ID,
 LIST_NAME,
 LIST_PRICE,
 N_COLUMNS
};

static int counter = 0;

void destroy(GtkWidget* widget, gpointer data){
	gtk_main_quit();
}
void greet(GtkWidget* widget, gpointer data)
{
	g_print("Welcome to GTK\n");
	g_print("%s clicked %d times\n",(char*)data, ++counter);
}
void lookup(){
	GtkListStore* store;
	GtkWidget* list, *window2,*vbox,*label;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeSelection* selection;
	sqlite3* db;
	char* err_msg = 0;
	
	//window 
	window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window2),"view");
	gtk_window_set_default_size(GTK_WINDOW(window2), 512,512);
	gtk_window_set_position(GTK_WINDOW(window2), GTK_WIN_POS_CENTER);
	g_signal_connect(window2, "destroy", G_CALLBACK(gtk_main_quit),NULL);
	//db
	int rc = sqlite3_open("test.db",&db);
	char* query = "SELECT * FROM MENU;";
	rc = sqlite3_exec(db,query,callback,0,&err_msg);
	if(rc != SQLITE_OK){
		perror("db");
		sqlite3_free(err_msg);
		sqlite3_close(db);
		exit(1);
	}

	//create list_view
	list = gtk_tree_view_new();
	//columns with rendering
	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("ID", renderer,"text",LIST_ID,
			NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("NAME",renderer,"text",LIST_NAME,NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("PRICE",renderer,"text",LIST_PRICE,NULL);
	g_object_unref(store);
	//setup the ui
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);
	vbox = gtk_vbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
	label = gtk_label_new("");
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);
	gtk_container_add(GTK_CONTAINER(window2), vbox);
	
	gtk_widget_show_all(window2);
	gtk_main();
	
	
}
void insert_data(){
}
void delete_data(){
}
void modify_data(){
}


int main(int argc, char* argv[]){

        gtk_init(&argc,&argv);
	//INIT WIDGET PART
        GtkWidget* window;
	GtkWidget* frame;
	GtkWidget* button1;
	GtkWidget* button2;
	GtkWidget* button3;
	GtkWidget* button4;
	//DB
	sqlite3 *db;
	char* err_msg = 0; 
	int rc = sqlite3_open("test.db", &db);
	if(rc != SQLITE_OK){
		perror("DB ");
		sqlite3_close(db);
		return 1;
	}
	char *sql = "DROP TABLE IF EXISTS MENU;"
		    "CREATE TABLE MENU(id int, name varchar(20), price int);"
		    "INSERT INTO MENU VALUES(1,'pasta',5000);"
		    "INSERT INTO MENU VALUES(2,'pizza',10000);";
	rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if(rc != SQLITE_OK){
		perror("exec");
		sqlite3_close(db);
		return 1;
	}
	sqlite3_close(db); 
	//LABELING PART
		//WINDOW
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "stock manager");
	gtk_window_set_default_size(GTK_WINDOW(window), 230,150);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
		//frame
	frame = gtk_fixed_new();
		//button
	button1 = gtk_button_new_with_label("INSERT DATA");
	gtk_widget_set_size_request(button1, 100, 50);
	button2 = gtk_button_new_with_label("DELETE DATA");
	gtk_widget_set_size_request(button2, 100, 50);
	button3 = gtk_button_new_with_label("MODIFYING DATA");
	gtk_widget_set_size_request(button3, 100, 50);
	button4 = gtk_button_new_with_label("LOOKUP DATA");
	gtk_widget_set_size_request(button4, 200, 50);
	//SIGNAL_PART
	g_signal_connect(window,"delete-event",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(GTK_OBJECT(button1), "clicked", G_CALLBACK(greet),"button");
	g_signal_connect(GTK_OBJECT(button4),"clicked", G_CALLBACK(lookup),"button");
	//CONTAINER PART
	gtk_container_add(GTK_CONTAINER(window), frame);
	gtk_fixed_put(GTK_FIXED(frame),button1,50, 20);
	gtk_fixed_put(GTK_FIXED(frame),button2,50, 60);
	gtk_fixed_put(GTK_FIXED(frame),button3,50, 100);
	gtk_fixed_put(GTK_FIXED(frame),button4,200,200);
	gtk_widget_show_all(window);
        gtk_main();
        return 0;


}

int callback(int argc, char **argv, char **azColName){
	GtkTreeIter iter;
	GtkListStore * list_store;
	list_store = gtk_list_store_new(LIST_ID,LIST_NAME,LIST_PRICE,N_COLUMNS);
	for(int i = 0; i < argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	gtk_list_store_append(list_store, &iter);
	gtk_list_store_set(list_store, &iter, LIST_ID, argv[0],
			LIST_NAME, argv[1],
			LIST_PRICE, argv[2],
			-1);
	return 0;
}
