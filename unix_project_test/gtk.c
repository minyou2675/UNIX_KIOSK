#include <gtk/gtk.h>
#include <sqlite3.h>
#include <stdio.h>
//gcc -o gtk gtk.c `pkg-config --libs --clfags gtk+-2.0`

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
	g_signal_connect(GTK_OBJECT(button1), "clicked", G_CALLBACK(greet),"butto	n");
	//CONTAINER PART
	gtk_container_add(GTK_CONTAINER(window), frame);
	gtk_fixed_put(GTK_FIXED(frame),button1,50, 20);
	gtk_fixed_put(GTK_FIXED(frame),button2,50, 60);
	gtk_fixed_put(GTK_FIXED(frame),button3,50, 100);
	gtk_fixed_put(GTK_FIXED(frame),button4,200,200);
	//gtk_container_add(GTK_CONTAINER(window), frame);
	//gtk_container_add(GTK_CONTAINER(window), button1);
	//gtk_container_add(GTK_CONTAINER(window), button2);
	//gtk_container_add(GTK_CONTAINER(window), button3);
	gtk_widget_show_all(window);
        gtk_main();
        return 0;


}
