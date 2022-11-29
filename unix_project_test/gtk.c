#include <gtk/gtk.h>

static int counter = 0;

void destroy(GtkWidget* widget, gpointer data){
	gtk_main_quit();
}
void greet(GtkWidget* widget, gpointer data)
{
	g_print("Welcome to GTK\n");
	g_print("%s clicked %d times\n",(char*)data, ++counter);
}

int main(int argc, char* argv[]){

        gtk_init(&argc,&argv);
        GtkWidget* window;
	GtkWidget* button;
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	button = gtk_button_new_with_label("Click Me!");
	g_signal_connect(window,"delete-event",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(GTK_OBJECT(button), "clicked", G_CALLBACK(greet),"butto	n");
	gtk_container_add(GTK_CONTAINER(window), button);
	gtk_widget_show_all(window);
        gtk_main();
        return 0;





















}
