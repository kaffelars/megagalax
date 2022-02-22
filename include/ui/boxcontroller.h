#ifndef BOXCONTROLLER_H
#define BOXCONTROLLER_H


namespace boxcontroller
{
    struct box
    {
        int id;
        std::string title;
        std::string text;
        boxtype type;
        bool removethis {false};
    };

    void addokaybox(std::string title, std::string text);

    void updateboxes();
};

#endif // BOXCONTROLLER_H
