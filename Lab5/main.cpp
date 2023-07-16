#include "mainwindow.h"
#include "ui_mainwindow.h"

HANDLE* handleArr;
DWORD* threadIDArr;
QString str;
std::string data = " Oleksandr Babilya 13503821";

std::ofstream fileNoSync("NoSync.txt", std::ios::app),
	fileCriticalSection("CriticalSection.txt", std::ios::app),
	fileWaitingTimers("WaitingTimers.txt", std::ios::app);
	
QString* statusArr;
5float* processTime;
CRITICAL_SECTION cs;
HANDLE hTimer = NULL;
DWORD dWaitForSingleObject;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->spinBox->setMinimum(1);
	ui->spinBox->setMaximum(16);
}

MainWindow::~MainWindow()
{
	delete ui;
	DeleteCriticalSection(&cs);
	CloseHandle(hTimer);
}

DWORD WINAPI funcNoSync(LPVOID lpParameter)
{
	int numberOfIterations = *(int*)lpParameter;
	
	for(int i = 0;i < (numberOfIterations); ++i)
		for (int j = 0;j < data.length();++j)
			fileNoSync<<data[j];
	fileNoSync.close();
	return 0;
}

DWORD WINAPI funcCriticalSection(LPVOID lpParameter)
{
	int numberOfIterations = *(int*)lpParameter;
	EnterCriticalSection(&cs);
	
	for(int i = 0;i < (numberOfIterations); ++i)
		for (int j = 0;j < data.length();++j)
		fileCriticalSection<<data[j];
		
	LeaveCriticalSection(&cs);
	fileCriticalSection.close();
	return 0;
}

DWORD WINAPI funcWaitingTimers(LPVOID lpParameter)
{
	dWaitForSingleObject = WaitForSingleObject(hTimer, INFINITE);
	int numberOfIterations = *(int*)lpParameter;
	
	for(int i = 0;i < (numberOfIterations); ++i)
		for (int j = 0;j < data.length();++j)
			fileWaitingTimers<<data[j];
			
	fileWaitingTimers.close();
	return 0;
}
void display(QPlainTextEdit* textEdit,int size)
{
	QString priority,str;
	for(int i=0;i<size;i++)
	{
		int prio = GetThreadPriority(handleArr[i]);
		switch(prio)
		{
			case 1: priority = "Above normal";break;
			case -1: priority = "Below normal";break;
			case 0: priority = "Normal";break;
			case -15: priority = "Idle";break;
			case 2: priority = "Hight";break;
		}
		str += "\nTreadd № " + QString::number(i + 1) + " ID = " +
			QString::number(GetThreadId(handleArr[i]))
			+ " Time: "+QString::number(processTime[0]) + "
			ms"+"\nStatus: " + statusArr[i] + " Priority: "+priority;
	}
	textEdit->setPlainText(str);
}

void MainWindow::on_createProcessButton_clicked()
{
	int size = ui->comboBox->currentText().toInt();
	int *iter = new int;
	*iter = ui->mValueLineEdit->text().toInt();
	handleArr = new HANDLE[size];
	threadIDArr = new DWORD[size];
	statusArr = new QString[size];
	processTime = new float[size];
	QString syncType = ui->syncComboBox->currentText();
	*iter /= size ;
	
	if(syncType=="No Sync")
	{
		QElapsedTimer timer;
		timer.start();
		
		for(int i=0;i<size;i++)
		{
			handleArr[i] = CreateThread(0, 0, funcNoSync, iter,0,
			&threadIDArr[0]);
			statusArr[i] = "Running";
		}
		
		WaitForMultipleObjects(size, handleArr, true, INFINITE);
		timer.nsecsElapsed();
		processTime[0] = timer.nsecsElapsed()/1000000.0;
	}
	if(syncType=="Critical Situations")
	{
		QElapsedTimer timer;
		timer.start();
		InitializeCriticalSection(&cs);
		
		for(int i=0;i<size;i++)
		{
			handleArr[i] = CreateThread(0, 0, funcCriticalSection, iter, 0,
			&threadIDArr[i]);
			statusArr[i] = "Running";
		}
		
		WaitForMultipleObjects(size, handleArr, true, INFINITE);
		timer.nsecsElapsed();
		processTime[0] = timer.nsecsElapsed()/10000.0;
	}
	else if(syncType=="Waiting Timers")
	{
		QElapsedTimer timer;
		timer.start();
		BOOL bSetWaitableTimer = false;
		LARGE_INTEGER liDueTime;
		liDueTime.QuadPart = -5000000LL;
		hTimer = CreateWaitableTimer(NULL, true, NULL);
		bSetWaitableTimer = SetWaitableTimer(hTimer, &liDueTime, 0, NULL,NULL, 0);
		
		for(int i=0;i<size;i++)
		{
			handleArr[i] = CreateThread(0, 0, funcWaitingTimers, iter, 0,
			&threadIDArr[i]);
			statusArr[i] = "Running";
			WaitForSingleObject(handleArr[i], INFINITE);
			dWaitForSingleObject = WaitForSingleObject(hTimer, INFINITE);
		}
		
		timer.nsecsElapsed();
		processTime[0] = timer.nsecsElapsed()/1000000.0;
	}
	display(ui->infoPlainTextEdit,ui->comboBox->currentText().toInt());
}

void MainWindow::on_runButton_clicked()
{
	//QElapsedTimer timer;
	//timer.start();
	ResumeThread(handleArr[ui->spinBox->value()-1]);
	statusArr[ui->spinBox->value()-1] = "Running";
	//WaitForSingleObject(handleArr[ui->spinBox->value()-1], INFINITE);
	//timer.nsecsElapsed();
	//processTime[0] = timer.nsecsElapsed()/1000000.0;
	display( ui->infoPlainTextEdit, ui->comboBox->currentText().toInt());
}

void MainWindow::on_pauseButton_clicked()
{
	SuspendThread(handleArr[ui->spinBox->value()-1]);
	statusArr[ui->spinBox->value()-1] = "Suspended";
	display( ui->infoPlainTextEdit, ui->comboBox->currentText().toInt());
}

void MainWindow::on_killButton_clicked()
{
	TerminateThread(handleArr[ui->spinBox->value()-1], 100);
	statusArr[ui->spinBox->value()-1] = "Terminated";
	display( ui->infoPlainTextEdit, ui->comboBox->currentText().toInt());
}

void MainWindow::on_setButton_clicked()
{
	int index=ui->spinBox->value();
	QString priority = ui->priorityComboBox->currentText();
	
	if(priority=="ABOVE NORMAL")
		SetThreadPriority(handleArr[index-1], 1);
	else if(priority=="BELOW NORMAL")
		SetThreadPriority(handleArr[index-1], -1);
	else if(priority=="HIGH")
		SetThreadPriority(handleArr[index-1], 2);
	else if(priority=="IDLE")
		SetThreadPriority(handleArr[index-1], -15);
	else if(priority=="NORMAL")
		SetThreadPriority(handleArr[index-1], 0);
	else
		SetThreadPriority(handleArr[index-1], 15);
		
	display(ui->infoPlainTextEdit,ui->comboBox->currentText().toInt());
}
