#include <QtCore/QCoreApplication>
#include <QtCore>
#include "rocksdb/db.h"
#include <string>
#include "include/curl/curl.h"
#define CURL_STATICLIB
//#pragma comment(lib, "C:/Users/arpandas/source/repos/codebox/codebox/lib/libcurl_a.lib")
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFile file("repos.txt");
    QFile file7("7repos.txt");
    QTextStream opt(stdout);
    rocksdb::Options optns;
    optns.create_if_missing = true;
    rocksdb::DB* db;
    rocksdb::Status status = rocksdb::DB::Open(optns, "repos.db", &db);
    rocksdb::DB* db7;
    rocksdb::Status status7 = rocksdb::DB::Open(optns, "7repos.db", &db7);
    QString cmd = QString(argv[1]);
    if (cmd == "-ur") {

        file.setPermissions(QFile::WriteOwner | QFile::ReadOwner);
        file.open(QIODevice::ReadOnly);
        QStringList lines = QString(file.readAll()).split("\n");
        QString os = lines[0].trimmed();
        for (int i = 1; i < lines.size(); i++)
        {
            QString line = lines[i];
            QStringList parts = line.split("@");
            QString name = parts[0];
            QString url = parts[1];
            //opt << os;

            url = url.append(os);
            url=url.trimmed();
            //opt << url;
            //opt.flush();
            db->Put(rocksdb::WriteOptions(), name.toStdString(), url.toStdString());

        }
    }
    else if (cmd == "-7ur") {

        file7.setPermissions(QFile::WriteOwner | QFile::ReadOwner);
        file7.open(QIODevice::ReadOnly);
        QStringList lines = QString(file7.readAll()).split("\n");
        QString os = lines[0].trimmed();
        for (int i = 1; i < lines.size(); i++)
        {
            QString line = lines[i];
            QStringList parts = line.split("@");
            QString name = parts[0];
            QString url = parts[1];
            //opt << os;

            url = url.append(os);
            url = url.trimmed();
            //opt << url;
            //opt.flush();
            db7->Put(rocksdb::WriteOptions(), name.toStdString(), url.toStdString());

        }
    }
	file.close();
    if (cmd == "-g") {
		std::string value;
        QStringList loc=QString(argv[2]).split(":");
		rocksdb::Status s = db->Get(rocksdb::ReadOptions(), loc[0].toStdString(), &value);
        //opt << value.c_str();
        if (s.ok()) {
            QString url = "https://";
            url = url.append(QString::fromStdString(value));
            std::string fnm=loc[1].toStdString()+".zip";
            FILE* fileopt= fopen(fnm.c_str(), "wb");
            url=url.append(loc[1]);
            url=url.append(".zip");
            opt << url;
            opt.flush();
			CURL *curl;
			CURLcode res;
			curl = curl_easy_init();
            if (curl) {
				curl_easy_setopt(curl, CURLOPT_URL, url.toStdString().c_str());
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, fileopt);
                //curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); 
                
                // curl_easy_setopt(curl, CURLOPT_)
                curl_easy_setopt(curl, CURLOPT_HEADER, 0);
				res = curl_easy_perform(curl);
				curl_easy_cleanup(curl);
                opt << res;
                opt.flush();
            }
            else {
                opt << "Error: ";
				opt << "Curl not initialized";
				opt << "\n";
                opt << curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL);
				opt.flush();
            }
			fclose(fileopt);
            QString command = "x64\\7za.exe x ";
            command = command.append(loc[1]+".zip -o.cbx/"+loc[1]);
            system(command.toStdString().c_str());
		}
        else {
            opt << "Error: ";
            opt << QString::fromStdString(s.ToString());
            opt<< "\n";
            opt.flush();
		}
        
	}
    else if (cmd == "-7g") {
        std::string value;
        QStringList loc = QString(argv[2]).split(":");
        rocksdb::Status s = db7->Get(rocksdb::ReadOptions(), loc[0].toStdString(), &value);
        //opt << value.c_str();
        if (s.ok()) {
            QString url = "https://";
            url = url.append(QString::fromStdString(value));
            std::string fnm = loc[1].toStdString() + ".7z";
            FILE* fileopt = fopen(fnm.c_str(), "wb");
            url = url.append(loc[1]);
            url = url.append(".7z");
            opt << url;
            opt.flush();
            CURL* curl;
            CURLcode res;
            curl = curl_easy_init();
            if (curl) {
                curl_easy_setopt(curl, CURLOPT_URL, url.toStdString().c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, fileopt);
                //curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); 

                // curl_easy_setopt(curl, CURLOPT_)
                curl_easy_setopt(curl, CURLOPT_HEADER, 0);
                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);
                opt << res;
                opt.flush();
            }
            else {
                opt << "Error: ";
                opt << "Curl not initialized";
                opt << "\n";
                opt << curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL);
                opt.flush();
            }
            fclose(fileopt);
            QString command = "x64\\7za.exe x ";
            command = command.append(loc[1] + ".7z -o.cbx/" + loc[1]);
            system(command.toStdString().c_str());
        }
        else {
            opt << "Error: ";
            opt << QString::fromStdString(s.ToString());
            opt << "\n";
            opt.flush();
        }

    }
    exit(0);

    return a.exec();
}
