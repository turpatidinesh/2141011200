#include<iostream>
#include<string>
#include<vector>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<cstring> // For strerror
#include<cstdlib> // For exit

using namespace std;

/// file_descriptor:
//  0 -> read (stdin)
//  1 -> write (stdout)
//  2 -> error (stderr)

vector<string> tokenizeCommand(string str, char delim) {
    vector<string> strTokens;
    string tmp = "";

    for (int i = 0; i < str.length(); i++) {
        if (str[i] == delim) {
            if (!tmp.empty()) strTokens.push_back(tmp);
            tmp = "";
        } else if (str[i] == '&') {
            if (!tmp.empty()) strTokens.push_back(tmp);
            strTokens.push_back("bgRunTriggered");
            return strTokens;
        } else if (str[i] == '\'') {
            // Ignore quotes
        } else {
            tmp += str[i];
        }
    }
    if (!tmp.empty()) strTokens.push_back(tmp);
    return strTokens;
}

vector<string> separateCommands(string str, char delim) {
    vector<string> strTokens;
    string tmp = "";
    bool foundPipe = false;

    for (int i = 0; i < str.length(); i++) {
        if (foundPipe) {
            foundPipe = false;
        } else if (str[i] == delim) {
            if (!tmp.empty()) strTokens.push_back(tmp);
            tmp = "";
            foundPipe = true;
        } else if (str[i] == ' ' && str[i+1] == delim) {
            // skip
        } else {
            tmp += str[i];
        }
    }
    if (!tmp.empty()) strTokens.push_back(tmp);
    return strTokens;
}

void shiftAndPop(vector<string>& vec) {
    if (!vec.empty()) {
        vec.erase(vec.begin());
    }
}

int main() {
    string cmd;
    pid_t ret_Val;
    int childCount = 0;
    bool pipesFlg = false;
    vector<string> history;
    int historyCommandsCounter = 0;

    while (true) {
        cout << "\n( Enter Command ) : ";
        getline(cin, cmd);
        if (cmd == "exit") break;

        if (cmd == "history") {
            int srNo = history.size();
            cout << "\n( Total Commands entered: " << historyCommandsCounter << " )\n";
            cout << "( Total history size: 10 )\n";
            cout << "( Current history size: " << history.size() << " )\n";
            cout << "\nShowing HISTORY...\n";
            for (int i = history.size() - 1; i >= 0; i--) {
                cout << srNo-- << " -> " << history[i] << endl;
            }
            continue;
        }

        if (cmd == "!!") {
            if (history.empty()) {
                cout << "history is empty\n";
                continue;
            }
            cmd = history.back();
            cout << "most recent command was " << cmd << endl << endl;
        }

        if (cmd[0] == '!' && cmd.length() > 1 && cmd[1] != '!') {
            try {
                int no = stoi(cmd.substr(1));
                if (no <= 0 || no > history.size()) {
                    cout << "command " << no << " not found in history! (//ERROR_404)\n";
                    continue;
                }
                cout << "command " << no << " will be executed...\n\n";
                cmd = history[no - 1];
            } catch (...) {
                cout << "Invalid history reference.\n";
                continue;
            }
        }

        historyCommandsCounter++;
        if (historyCommandsCounter > 10) {
            shiftAndPop(history);
        }
        history.push_back(cmd);

        vector<string> commands = separateCommands(cmd, '|');
        pipesFlg = (commands.size() > 1);

        int prev_pipe[2];
        int next_pipe[2];

        for (size_t i = 0; i < commands.size(); i++) {
            vector<string> tokens = tokenizeCommand(commands[i], ' ');
            bool bgRunFlag = false;

            if (!tokens.empty() && tokens.back() == "bgRunTriggered") {
                bgRunFlag = true;
                tokens.pop_back();
            }

            if (tokens.empty()) continue;

            if (tokens[0] == "cd") {
                if (tokens.size() < 2) {
                    cout << "Usage: cd <directory>" << endl;
                } else if (chdir(tokens[1].c_str()) != 0) {
                    perror("cd failed");
                }
                continue;
            }

            if (tokens[0] == "mkfifo") {
                if (tokens.size() < 2) {
                    cout << "Usage: mkfifo <filename>\n";
                } else if (mkfifo(tokens[1].c_str(), 0666) != 0) {
                    perror("mkfifo failed");
                } else {
                    cout << "named pipe: " << tokens[1] << " created successfully!\n";
                }
                continue;
            }

            if (i < commands.size() - 1 && pipe(next_pipe) == -1) {
                perror("pipe failed");
                exit(EXIT_FAILURE);
            }

            childCount++;
            ret_Val = fork();

            if (ret_Val < 0) {
                cerr << "\nFork() failed!\n";
            } else if (ret_Val == 0) {
                // CHILD
                for (size_t j = 0; j < tokens.size(); j++) {
                    if (tokens[j] == "<" && j + 1 < tokens.size()) {
                        int fd = open(tokens[j+1].c_str(), O_RDONLY);
                        if (fd < 0) perror("open <");
                        dup2(fd, 0);
                        close(fd);
                        tokens.erase(tokens.begin()+j, tokens.begin()+j+2);
                        j--;
                    } else if (tokens[j] == ">" && j + 1 < tokens.size()) {
                        int fd = open(tokens[j+1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        if (fd < 0) perror("open >");
                        dup2(fd, 1);
                        close(fd);
                        tokens.erase(tokens.begin()+j, tokens.begin()+j+2);
                        j--;
                    } else if (tokens[j] == "2>" && j + 1 < tokens.size()) {
                        int fd = open(tokens[j+1].c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
                        if (fd < 0) perror("open 2>");
                        dup2(fd, 2);
                        close(fd);
                        tokens.erase(tokens.begin()+j, tokens.begin()+j+2);
                        j--;
                    }
                }

                if (pipesFlg) {
                    if (i < commands.size() - 1) dup2(next_pipe[1], 1);
                    if (i > 0) dup2(prev_pipe[0], 0);
                }

                if (pipesFlg) {
                    close(prev_pipe[0]);
                    close(prev_pipe[1]);
                    close(next_pipe[0]);
                    close(next_pipe[1]);
                }

                char* args[tokens.size() + 1];
                for (size_t j = 0; j < tokens.size(); j++) {
                    args[j] = const_cast<char*>(tokens[j].c_str());
                }
                args[tokens.size()] = NULL;

                if (execvp(args[0], args) == -1) {
                    perror("execvp failed");
                    exit(EXIT_FAILURE);
                }
            } else {
                // PARENT
                if (pipesFlg) {
                    if (i < commands.size() - 1) close(next_pipe[1]);
                    if (i > 0) close(prev_pipe[0]);
                    prev_pipe[0] = next_pipe[0];
                    prev_pipe[1] = next_pipe[1];
                }

                if (!bgRunFlag) {
                    int status;
                    waitpid(ret_Val, &status, 0);
                    cout << "child with pid " << ret_Val << " terminated!\n";
                } else {
                    cout << "child running in background!\n";
                }
            }
        }
    }

    cout << "\n***** shell terminated! *****\n";
    return 0;
}

