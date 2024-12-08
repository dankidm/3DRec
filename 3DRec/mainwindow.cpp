#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImageSlice.h>
#include <vtkImageSliceMapper.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , renderWindow(vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New())
    , renderer(vtkSmartPointer<vtkRenderer>::New()) {
    ui->setupUi(this);

    // Настройка VTK виджета
    ui->vtkWidget->setRenderWindow(renderWindow);
    renderWindow->AddRenderer(renderer);

    // Подключение кнопки загрузки
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadDICOM);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onLoadDICOM() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open DICOM File"), "", tr("DICOM Files (*.dcm)"));
    if (fileName.isEmpty()) {
        return;
    }

    // Чтение DICOM файла
    vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader->SetFileName(fileName.toStdString().c_str());
    reader->Update();

    // Настройка цветовой карты
    vtkSmartPointer<vtkImageMapToWindowLevelColors> colorMap = vtkSmartPointer<vtkImageMapToWindowLevelColors>::New();
    colorMap->SetInputConnection(reader->GetOutputPort());
    colorMap->Update();

    // Создание маппера и слоя изображения
    vtkSmartPointer<vtkImageSliceMapper> imageMapper = vtkSmartPointer<vtkImageSliceMapper>::New();
    imageMapper->SetInputConnection(colorMap->GetOutputPort());

    vtkSmartPointer<vtkImageSlice> imageSlice = vtkSmartPointer<vtkImageSlice>::New();
    imageSlice->SetMapper(imageMapper);

    // Очистка сцены и добавление слоя
    renderer->RemoveAllViewProps();
    renderer->AddViewProp(imageSlice);
    renderer->ResetCamera();

    // Обновление окна
    ui->vtkWidget->renderWindow()->Render();
}
