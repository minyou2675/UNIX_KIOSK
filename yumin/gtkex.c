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

//ipc 관련 선언들
  //////////////IPC AREA////////////////////////////
char* pointingName; 
 struct mymsgbuf {
 long mtype;
 char mtext[10];
 };
key_t key;
 int msgid;
 struct mymsgbuf mesg;

//키오스크 메인화면 리스트 초기화
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


//리스트에 품목을 추가할수 있게 해주는 함수
void add_to_list(GtkWidget *list, const gchar *str, Data* data ) {
    
  GtkListStore *store;
  GtkTreeIter iter;
  
  //구조체의 주소는 따로보관하여 나중에 이름 및 가격의 참조를 용이하게 함
  dataArr[arrnum] = data;
  arrnum++;
  if(arrnum > MAXDATA)
  	printf("add_to_list() : MAXDATA OVERFLOW");
  
  store = GTK_LIST_STORE(gtk_tree_view_get_model
      (GTK_TREE_VIEW(list)));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
  

}

//리스트 항목을 한 번 클릭 했을 시 호출되는 함수
void on_changed(GtkWidget *widget, gpointer label) {
    
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar* value;
  
  //라벨의 내용을 품명으로 바꿔준다.

  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter)) {

    gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
    gtk_label_set_text(GTK_LABEL(label), value);
       g_free(value);
  }
}

//주문 버튼으로 품목과 수량을 전달시켜주는 함수 - 메시지 큐 이용
void press_order(GtkWidget *widget, GtkEntry* entry)
{
    char str[10] = "";

    //서버에서 데이터 언패킹이 용이하도록 규격을 맞춤
    g_print("\nOrder :::::::::: ");

    strcpy(str, pointingName);
    strcat(str, ",");
    strcat(str, gtk_entry_get_text(entry));
    g_print("s %s  :: ", str);

    //메시지 큐를 이용한 전송
mesg.mtype = 1;
 strcpy(mesg.mtext, str);

 if (msgsnd(msgid, (void *)&mesg, 10, IPC_NOWAIT) == -1) {
 perror("msgsnd");
 exit(1);
 }
}

//리스트의 항목을 더블 클릭 시 주문 창을 띄워주는 함수
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
    
    //호출시 어떤 항목인지 출력
    g_print ("activated: %s\n", string);
    pointingName = string;

    //삽입된 리스트에서 이름을 통해 실제 품목의 구조체를 추적
      for(int i = 0; i<arrnum; i++)
      {
      	if( strcmp(string, dataArr[i]->name) == 0 )
        {
          data = dataArr[i];
          break;
        }else
        	data = NULL;
      }

      //주문 창을 띄워주는 내용
      window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      gtk_window_set_title(GTK_WINDOW(window), data->name);
      gtk_window_set_default_size(GTK_WINDOW(window),150,100);
      gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
      
      grid = gtk_grid_new();
      gtk_container_add (GTK_CONTAINER(window), GTK_WIDGET(grid));

      //품명과 가격을 표시해준다
      buf = g_strdup_printf("NAME : %s\nPRICE : %d\n", data->name, data->price);
      namelabel = gtk_label_new(buf);
      gtk_grid_attach (GTK_GRID(grid), namelabel, 0, 0, 1, 1);
      entry = gtk_entry_new();
      gtk_grid_attach(GTK_GRID(grid),entry, 1, 2, 1, 1);


      //버튼 두 개를 주문, 취소로 나누어 부착시킴
      buttonO = gtk_button_new_with_label("Order");
      gtk_grid_attach(GTK_GRID(grid), buttonO, 0, 3, 1, 1);      
      buttonX = gtk_button_new_with_label("Cancel");

      gtk_grid_attach(GTK_GRID(grid), buttonX, 1, 3, 1, 1);      

//        system("ps -l");
      
      //각각의 버튼에 역할에 맞는 시그널(호출함수) 부여
      g_signal_connect(G_OBJECT(buttonX), "clicked", G_CALLBACK(gtk_window_close), (GtkWindow*) window);
    
      g_signal_connect(G_OBJECT(buttonO), "clicked", G_CALLBACK(press_order), (GtkEntry*) entry);
      
      gtk_widget_show_all(window);
      
      
    
     
}

void refreshDataArray()
{
    int len;
    struct mymsgbuf inmsg;

//    char* p_token;
  //  char* p_next_token;

    //char* np_token;
   // char* np_next_token;
    
    len = msgrcv(msgid, &inmsg, 10, 0, 0);
    g_print("rcv = %s\n", inmsg.mtext, len);
    
    
/////////get data in format
/*
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
    }*/
}



int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *list;

  GtkWidget *sw;
  GtkWidget *vbox;
  GtkWidget *label;
  GtkTreeSelection *selection; 
    
  //임시 품목 설정
  Data pasta, pizza;
  pasta.name = "pasta";
  pasta.price = 123;
  pizza.name = "pizza";
  pizza.price = 456;

  //메시지 큐 설정을 위한 키파일 생성
  key = ftok("keyfile", 1);
 msgid = msgget(key, IPC_CREAT|0644);
 if (msgid == -1) {
 perror("msgget");
 exit(1);
 }

 //gtk 시동 함수
  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  list = gtk_tree_view_new();

  //기본 창의 이름과 사이즈를 조정해준다.
  gtk_window_set_title(GTK_WINDOW(window), "Item List");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 400);

  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

  vbox = gtk_box_new(FALSE, 0);


  label = gtk_label_new("");
  //스크롤 기능 추가
  sw = gtk_scrolled_window_new (NULL, NULL);
      gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
                                           GTK_SHADOW_ETCHED_IN);
      gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
                                      GTK_POLICY_AUTOMATIC,
                                      GTK_POLICY_AUTOMATIC);
      gtk_box_pack_start (GTK_BOX (vbox), sw, TRUE, TRUE, 0); 
  

    //생성한 콘텐츠들을 창에 붙여준다.
  gtk_container_add(GTK_CONTAINER(window), vbox);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);
  gtk_container_add(GTK_CONTAINER(sw), list);
//////////////////
//
//리스트에 아이템 추가 과정(임시)
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
  

  //품목 선택 관련 상호 작용을 관리하는 영역
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
