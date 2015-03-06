/**
 *  Header file for nework communications exercise
 *
 *  Advanced Programming course
 *  Ort Braude College
 *
 */

#define B_PORT_NUM 12345	/* port number for server */

#define B_ENT_CNT  64		/* number of entries in board maintained by server*/

#define B_DATE_LEN 12 		/* length of the date area in input */
#define B_CAT_LEN  18 		/* length of 'category' field in input */
#define B_TEXT_LEN 200		/* max length of 'text' field in input */

/*
 *  structure for indicating a date
 */
typedef struct
{
    int  day;
    int  month;
    int  year;
}
    date_t;

/**
 * The structure of each classified entry. The board consistes of an
 * array of such entries.
 */
typedef struct board_ent
{
    date_t b_date;
    char   b_category[B_CAT_LEN + 1];
    char   b_text[B_TEXT_LEN + 1];
}
    board_ent_t;


/*
 *  Definitions regarding the protocol used for communications between
 *  the server and its clients:
 *  1. brdent -  a client responsible for reading the input file and sending one
 *               classified entry per second to the server.
 *  2. brdcat -  a client requesting a list of entries pertaining to a given category.
 *  3. brddate - a client requesting entries issued within a given range of dates.
 */

/*
 *  message types
 */
#define B_TYPE_OK    0
#define B_TYPE_ENT   1
#define B_TYPE_CAT   2
#define B_TYPE_DATE  3
#define B_TYPE_CNT   4

/*
 *  message formats: an OK response message
 */
typedef struct msg_ok
{
    int         m_type;	// = B_TYPE_OK
}
    msg_ok_t;

/*
 *  message formats: a message from brdent client containing a single classified entry
 */
typedef struct msg_ent
{
    int         m_type;	// = B_TYPE_ENT
    board_ent_t m_entry;
}
    msg_ent_t;

/*
 *  message formats: a message from brdcat client containing a category
 */
typedef struct msg_cat
{
    int         m_type;		// = B_TYPE_CAT
    char        m_cat[B_CAT_LEN];
}
    msg_cat_t;

/*
 *  message formats: a message from brddate client containing a date range
 */
typedef struct msg_date
{
    int         m_type;		// = B_TYPE_DATE
    date_t      m_start;	// start date
}
    msg_date_t;

/*
 *  message formats: a message from brdserv indicating how many additional
 *                   msg_ent messages are to follow
 */
typedef struct msg_cnt
{
    int         m_type;		// = B_TYPE_cnT
    int         m_count; 	// number of entries to follow
}
    msg_cnt_t;





