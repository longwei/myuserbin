#ifndef DB_OBJECT  
#define DB_OBJECT  
#include <map>


# define ls_slots  
# define ls_signals protected  
# define ls_emit

class Object;

struct MetaObject  
{  
    const char * sig_names;  
    const char * slts_names;  
    static void active(Object * sender, int idx);  
};

struct Connection  
{  
    Object * receiver;  
    int method;  
};  

typedef std::multimap<int, Connection> ConnectionMap;  
typedef std::multimap<int, Connection>::iterator ConnectionMapIt;  

class Object  
{  
    static MetaObject meta;  
    void metacall(int idx);  
public:  
    Object();  
    virtual ~Object();  
    static void connect(Object*, const char*, Object*, const char*);  
    void testSignal();  
ls_signals:  
    void sig1();  
public ls_slots:  
    void slot1();  
friend class MetaObject;  
private:  
     ConnectionMap connections;  
};  
#endif
