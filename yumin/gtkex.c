#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/msg.h>

#define MAXDATA 100

enum {
  LIST_ITEM,
  N_COLUMNS
};

typedef struct {
  char* name;
  int price;
  } Data;
  
Data* dataArr[MAXDATA];
int arrnum =0;


  //////////////IPC AREA////////////////////////////
char* pointingName; 
 struct mymsgbuf {
 long mtype;
 char mtext[BUFSIZ];
 };
key_t key;
 int msgid;
 struct mymsgbuf mesg;


void init_list(GtkWidget *list) {

  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkListStore *store;

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes("NAME",
          renderer, "text", LIST_ITEM, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
  

  store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(list), 
      GTK_TREE_MODEL(store));

  g_object_unref(store);
}

void add_to_list(GtkWidget *list, const gchar *str, Data* data ) {
    
  GtkListStore *store;
  GtkTreeIter iter;
  
  dataArr[arrnum] = data;
  arrnum++;
  if(arrnum > MAXDATA)
  	printf("add_to_list() : MAXDATA OVERFLOW");
  
  store = GTK_LIST_STORE(gtk_tree_view_get_model
      (GTK_TREE_VIEW(list)));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
  

}

void on_changed(GtkWidget *widget, gpointer label) {
    
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar* value;
  

  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter)) {

    gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
    gtk_label_set_text(GTK_LABEL(label), value);
       g_free(value);
  }
}

void press_order(GtkWidget *widget, GtkEntry* entry)
{
    char str[BUFSIZ] = "";


    g_print("\nOrder :::::::::: ");

    strcpy(str, pointingName);
    strcat(str, ",");
    strcat(str, gtk_entry_get_text(entry));
    g_print("s %s  :: ", str);

mesg.mtype = 1;
 strcpy(mesg.mtext, str);

 if (msgsnd(msgid, (void *)&mesg, BUFSIZ, IPC_NOWAIT) == -1) {
 perror("msgsnd");
 exit(1);
 }
}

void replace_tab(GtkTreeView *tree_view, GtkTreePath *path, gpointer user_data)
{
    gchar *string;
    GtkTreeIter iter;
    GtkTreeModel *model = gtk_tree_view_get_model (tree_view);
    gtk_tree_model_get_iter(model, &iter, path); 
    gtk_tree_model_get (model, &iter, 0, &string, -1);
    
    Data* data;
    char* buf;
  
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *buttonO;
    GtkWidget *buttonX;
    GtkWidget *namelabel;
    GtkWidget *entry;

    g_print ("activated: %s\n", string);
    pointingName = string;

      for(int i = 0; i<arrnum; i++)
      {
      	if( strcmp(string, dataArr[i]->name) == 0 )
        {
          data = dataArr[i];
          break;
        }else
        	data = NULL;
      }
      window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      gtk_window_set_title(GTK_WINDOW(window), data->name);
      gtk_window_set_default_size(GTK_WINDOW(window),150,100);
      gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
      
      grid = gtk_grid_new();
      gtk_container_add (GTK_CONTAINER(window), GTK_WIDGET(grid));

      buf = g_strdup_printf("NAME : %s\nPRICE : %d\n", data->name, data->price);
      namelabel = gtk_label_new(buf);
      gtk_grid_attach (GTK_GRID(grid), namelabel, 0, 0, 1, 1);
      entry = gtk_entry_new();
      gtk_grid_attach(GTK_GRID(grid),entry, 1, 2, 1, 1);

      buttonO = gtk_button_new_with_label("Order");
      gtk_grid_attach(GTK_GRID(grid), buttonO, 0, 3, 1, 1);      
      buttonX = gtk_button_new_with_label("Cancel");

      gtk_grid_attach(GTK_GRID(grid), buttonX, 1, 3, 1, 1);      

//        system("ps -l");
      
      g_signal_connect(G_OBJECT(buttonX), "clicked", G_CALLBACK(gtk_window_close), (GtkWindow*) window);
    
      g_signal_connect(G_OBJECT(buttonO), "clicked", G_CALLBACK(press_order), (GtkEntry*) entry);
      
      gtk_widget_show_all(window);
      
      
    
     
}

void refreshDataArray()
{
    int pd, n;
    char inmsg[1024];

    char* p_token;
    char* p_next_token;

    char* np_token;
    char* np_next_token;
    
    if((pd = open("./menu_pipe", O_RDONLY)) == -1)
    {
        perror("openfifoin");
        exit(1);
    }

    n=read(pd, inmsg, 1024);
    
    
    if(n==-1)
    {
        perror("read");
        exit(1);
    }
    close(pd);

    g_print("%s\n", inmsg);
/////////get data in format

    p_token = strtok_r(inmsg, "\n", &p_next_token);
    for(int i = 1; p_token != NULL; i++)
    {
        if((np_token=strtok_r(p_token, " ", &np_next_token)) != NULL)
        {
            dataArr[i]->name = np_token;
            dataArr[i]->price = atoi(np_next_token);
            g_print("data%d : %s, %d", i, dataArr[i]->name, dataArr[i]->price);
        }
        p_token = strtok_r(NULL, "\n", &p_next_token);
    }
}


int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *list;

  GtkWidget *sw;
  GtkWidget *vbox;
  GtkWidget *label;
  GtkTreeSelection *selection; 

  Data pasta, pizza;
  pasta.name = "pasta";
  pasta.price = 123;
  pizza.name = "pizza";
  pizza.price = 456;

  key = ftok("keyfile", 1);
 msgid = msgget(key, IPC_CREAT|0644);
 if (msgid == -1) {
 perror("msgget");
 exit(1);
 }

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  list = gtk_tree_view_new();

  gtk_window_set_title(GTK_WINDOW(window), "Item List");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 400);

  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

  vbox = gtk_box_new(FALSE, 0);


  label = gtk_label_new("");
  
  sw = gtk_scrolled_window_new (NULL, NULL);
      gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
                                           GTK_SHADOW_ETCHED_IN);
      gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
                                      GTK_POLICY_AUTOMATIC,
                                      GTK_POLICY_AUTOMATIC);
      gtk_box_pack_start (GTK_BOX (vbox), sw, TRUE, TRUE, 0); 
  
  gtk_container_add(GTK_CONTAINER(window), vbox);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);
  gtk_container_add(GTK_CONTAINER(sw), list);
//////////////////
  init_list(list);
  add_to_list(list, pasta.name, &pasta);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
  add_to_list(list, pizza.name, &pizza);
 // add_to_list(list, pasta.name);
  
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));


  g_signal_connect(list, "row-activated", G_CALLBACK(replace_tab), NULL);

  g_signal_connect(selection, "changed", 
      G_CALLBACK(on_changed), label);

  g_signal_connect(G_OBJECT (window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
