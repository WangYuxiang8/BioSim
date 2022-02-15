#include "BioSim.h"
#include <QtWidgets/QApplication>

#include "inputBioInfo.h"
#include "image.h"
#include "model.h"
#include "present.h"

int main(int argc, char *argv[])
{
    try {
        QApplication a(argc, argv);
        Model my_model(inputBioInfo::read(argv[1]));
        Present my_present(my_model);
        BioSim my_view(my_present);
        my_present.set_view(my_view);
        my_view.start();
        my_view.show();
        return a.exec();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}
