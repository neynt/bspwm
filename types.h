#ifndef _TYPES_H
#define _TYPES_H

#include <stdbool.h>
#include <xcb/xcb.h>
#include <xcb/randr.h>
#include <xcb/xcb_event.h>
#include "helpers.h"

#define MISSING_VALUE        "N/A"

typedef enum {
    TYPE_HORIZONTAL,
    TYPE_VERTICAL
} split_type_t;

typedef enum {
    MODE_AUTOMATIC,
    MODE_MANUAL
} split_mode_t;

typedef enum {
    MOVE_PULL,
    MOVE_PUSH
} fence_move_t;

typedef enum {
    CHANGE_INCREASE,
    CHANGE_DECREASE
} value_change_t;

typedef enum {
    CLIENT_TYPE_ALL,
    CLIENT_TYPE_FLOATING,
    CLIENT_TYPE_TILED
} client_type_t;

typedef enum {
    CLIENT_CLASS_ALL,
    CLIENT_CLASS_EQUAL,
    CLIENT_CLASS_DIFFER
} client_class_t;

typedef enum {
    CLIENT_MODE_ALL,
    CLIENT_MODE_AUTOMATIC,
    CLIENT_MODE_MANUAL
} client_mode_t;

typedef enum {
    CLIENT_URGENCY_ALL,
    CLIENT_URGENCY_ON,
    CLIENT_URGENCY_OFF
} client_urgency_t;

typedef struct {
    client_type_t type;
    client_class_t class;
    client_mode_t mode;
    client_urgency_t urgency;
} client_select_t;

typedef enum {
    ALTER_NONE,
    ALTER_TOGGLE,
    ALTER_SET
} state_alter_t;

typedef enum {
    CYCLE_NEXT,
    CYCLE_PREV
} cycle_dir_t;

typedef enum {
    CIRCULATE_FORWARD,
    CIRCULATE_BACKWARD
} circulate_dir_t;

typedef enum {
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT,
    DIR_UP
} direction_t;

typedef enum {
    CORNER_TOP_LEFT,
    CORNER_TOP_RIGHT,
    CORNER_BOTTOM_RIGHT,
    CORNER_BOTTOM_LEFT
} corner_t;

typedef enum {
    SIDE_LEFT,
    SIDE_TOP,
    SIDE_RIGHT,
    SIDE_BOTTOM
} side_t;

typedef enum {
    ACTION_NONE,
    ACTION_FOCUS,
    ACTION_MOVE,
    ACTION_RESIZE_SIDE,
    ACTION_RESIZE_CORNER
} pointer_action_t;

typedef enum {
    LAYOUT_TILED,
    LAYOUT_MONOCLE
} layout_t;

typedef enum {
    FLIP_HORIZONTAL,
    FLIP_VERTICAL
} flip_t;

typedef enum {
    DESKTOP_STATUS_ALL,
    DESKTOP_STATUS_FREE,
    DESKTOP_STATUS_OCCUPIED
} desktop_status_t;

typedef enum {
    DESKTOP_URGENCY_ALL,
    DESKTOP_URGENCY_ON,
    DESKTOP_URGENCY_OFF
} desktop_urgency_t;

typedef struct {
    desktop_status_t status;
    desktop_urgency_t urgency;
} desktop_select_t;

typedef struct {
    xcb_window_t window;
    char class_name[SMALEN];
    unsigned int border_width;
    bool floating;
    bool transient;    /* transient window are always floating */
    bool fullscreen;
    bool locked;       /* protects window from being closed */
    bool urgent;
    bool icccm_focus;
    xcb_rectangle_t floating_rectangle;
    xcb_rectangle_t tiled_rectangle;
} client_t;

typedef struct node_t node_t;
struct node_t {
    split_type_t split_type;
    double split_ratio;
    split_mode_t split_mode;
    direction_t split_dir;
    int birth_rotation;
    xcb_rectangle_t rectangle;
    bool vacant;          /* vacant nodes only hold floating clients */
    node_t *first_child;
    node_t *second_child;
    node_t *parent;
    client_t *client;     /* NULL except for leaves */
};

typedef struct node_list_t node_list_t;
struct node_list_t {
    node_t *node;
    bool latest;
    node_list_t *prev;
    node_list_t *next;
};

typedef struct {
    node_list_t *head;
    node_list_t *tail;
} focus_history_t;

typedef struct desktop_t desktop_t;
struct desktop_t {
    char name[SMALEN];
    layout_t layout;
    node_t *root;
    node_t *focus;
    focus_history_t *history;
    desktop_t *prev;
    desktop_t *next;
    int window_gap;
};

typedef struct monitor_t monitor_t;
struct monitor_t {
    char name[SMALEN];
    xcb_randr_output_t id;
    xcb_rectangle_t rectangle;
    xcb_window_t root;
    bool wired;
    int top_padding;
    int right_padding;
    int bottom_padding;
    int left_padding;
    desktop_t *desk;
    desktop_t *last_desk;
    desktop_t *desk_head;
    desktop_t *desk_tail;
    monitor_t *prev;
    monitor_t *next;
};

typedef struct {
    monitor_t *monitor;
    desktop_t *desktop;
    node_t *node;
} coordinates_t;

typedef struct {
    char name[SMALEN];
} rule_cause_t;

typedef struct {
    bool floating;
    bool fullscreen;
    bool locked;
    bool follow;
    bool focus;
    bool unmanage;
    char desc[MAXLEN];
} rule_effect_t;

typedef struct rule_t rule_t;
struct rule_t {
    unsigned int uid;
    bool one_shot;
    rule_cause_t cause;
    rule_effect_t effect;
    rule_t *prev;
    rule_t *next;
};

typedef struct {
    xcb_point_t position;
    pointer_action_t action;
    xcb_rectangle_t rectangle;
    node_t *vertical_fence;
    node_t *horizontal_fence;
    monitor_t *monitor;
    desktop_t *desktop;
    node_t *node;
    client_t *client;
    xcb_window_t window;
    bool is_tiled;
    double vertical_ratio;
    double horizontal_ratio;
    corner_t corner;
    side_t side;
} pointer_state_t;

typedef struct {
    node_t *fence;
    unsigned int distance;
} fence_distance_t;

#endif
