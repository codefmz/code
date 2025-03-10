#include "gtest/gtest.h"
#include "msg.h"

class msgTest : public ::testing::Test {
public:
    msgTest() {
        std::cout << "msgTest" << std::endl;
    }
    ~msgTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

extern int g_argc;
extern char **g_argv;

TEST_F(msgTest, test1) {
    key_t key;
    int msgqueue_id;
    struct mymsgbuf qbuf;
    int argc = g_argc;
    char ** argv = g_argv;

    if(argc == 1) {
        usage();
    }

    /* Create unique key via call to ftok() */
    key = ftok(".", 'm');

    /* Open the queue - create if necessary */
    if((msgqueue_id = msgget(key, IPC_CREAT|0660)) == -1) {
        perror("msgget");
        exit(1);
    }

    switch(tolower(argv[1][0])) {
    case 's':
        send_message(msgqueue_id, (struct mymsgbuf *)&qbuf, atol(argv[2]), argv[3]);
        break;
    case 'r':
        read_message(msgqueue_id, &qbuf, atol(argv[2]));
        break;
    case 'd':
        remove_queue(msgqueue_id);
        break;
    case 'm':
        change_queue_mode(msgqueue_id, argv[2]);
        break;
    default:
        usage();
    }
}