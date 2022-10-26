#include "title.h"

Title::Title(const QString &title, const QString &title_path, int ischapter, float progress, int chap_count, int num)
    :title(title),title_path(title_path),ischapter(ischapter)
    ,progress(progress),chap_page_count(chap_count),num(num)
{

}

Chapter::Chapter(const QString &title, const QString &chapter
                 , const QString &chap_path, int page_count)
      :title(title),chapter(chapter)
      ,chap_path(chap_path),page_count(page_count)
{

}
