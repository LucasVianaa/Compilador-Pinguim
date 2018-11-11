#include <string>
using namespace std;
class ResultType{
private:
    bool accept;
    string text;
    int line;
    int column;
    string error;
public:
    ResultType(bool acc, string t, int l, int c);
    ResultType(bool acc, string t, int l, int c, string err);
    ResultType();
    bool getAccept();
    string getText();
    int getLine();
    int getColumn();

    void setAccept(bool a);
    void setText(string t);
    void setLine(int l);
    void setColumn(int c);
};