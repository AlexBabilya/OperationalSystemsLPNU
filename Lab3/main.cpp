#include "mainwindow.h"
#include "ui_mainwindow.h"

PROCESS_INFORMATION* piArr;
float* processTime;
QString* statusArray;
int* PIBArray;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->spinBox->setMinimum(1);
	ui->spinBox->setMaximum(8);
}

MainWindow::~MainWindow()
	{
	for(int i = 0; i < ui->comboBox->currentText().toInt(); ++i)
		{
		CloseHandle((piArr[i]).hProcess);
		}
	delete ui;
	}
	
void display(QPlainTextEdit* textEdit,int size)
{
	QString priority,str;
	for(int i=0;i<size;i++)
		{
		switch(GetPriorityClass(piArr[i].hProcess))
		{
			case 32768:priority="Above Normal";break;
			case 16384:priority="Below Normal";break;
			case 256:priority="Realtime";break;
			case 128:priority="High";break;
			case 64:priority="Idle";break;
			case 32:priority="Normal";break;
		}
		str+="\nProcess № "+QString::number(i+1)+" ID =
		"+QString::number(GetProcessId(piArr[i].hProcess))
		+" Time: "+QString::number(processTime[i])+" ms"+"\nStatus:
		"+statusArray[i]+ " Priority: "+priority;
		}
	textEdit->setPlainText(str);
}

void MainWindow::on_createProcessButton_clicked()
{
	QString str;
	int size=ui->comboBox->currentText().toInt();
	piArr = new PROCESS_INFORMATION[size];
	processTime = new float[size];
	statusArray=new QString[size];
	PIBArray = new int[size];
	long long int kernel, user, m1, m2, result = 0;
	
	for(int i=0;i<size;i++)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		std::wstring CommandLineOne(L"D://QTReal//Apps//OSLab//app//Debug//app.exe1000 10000 ");
		
		if (!CreateProcess(NULL, &CommandLineOne[0], NULL, NULL, FALSE, 0, NULL,NULL, &si, &piArr[i]))
			ui->errorLabel->setText("Error");
			
		kernel = 0;
		user = 0;
		result = 0;
		
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		GetProcessTimes(piArr[i].hProcess, (FILETIME*)&m1, (FILETIME*)&m2,(FILETIME*)&kernel, (FILETIME*)&user);
		qApp->processEvents();
		result = kernel+user;
		processTime[i] = result/10000.0;
		statusArray[i] = "Running";
	}
	display(ui->infoPlainTextEdit,size);
}

void MainWindow::on_killButton_clicked()
{
	int index=ui->spinBox->value();
	TerminateProcess(piArr[index-1].hProcess,100);
	statusArray[index-1]="Terminated";
	display(ui->infoPlainTextEdit,ui->comboBox->currentText().toInt());
}

void MainWindow::on_pauseButton_clicked()
{
	int index=ui->spinBox->value();
	SuspendThread(piArr[index-1].hThread);
	statusArray[index-1]="Suspended";
	display(ui->infoPlainTextEdit,ui->comboBox->currentText().toInt());
}

void MainWindow::on_runButton_clicked()
{
	int index=ui->spinBox->value();
	ResumeThread(piArr[index-1].hThread);
	statusArray[index-1]="Running";
	display(ui->infoPlainTextEdit,ui->comboBox->currentText().toInt());
}

void MainWindow::on_setButton_clicked()
{
	int index=ui->spinBox->value(),size=ui->comboBox->currentText().toInt();
	QString priority = ui->priorityComboBox->currentText(),str;
	
	if(priority=="ABOVE NORMAL")
		SetPriorityClass(piArr[index-1].hProcess , ABOVE_NORMAL_PRIORITY_CLASS);
	else if(priority=="BELOW NORMAL")
		SetPriorityClass(piArr[index-1].hProcess , BELOW_NORMAL_PRIORITY_CLASS);
	else if(priority=="HIGH")
		SetPriorityClass(piArr[index-1].hProcess , HIGH_PRIORITY_CLASS);
	else if(priority=="IDLE")
		SetPriorityClass(piArr[index-1].hProcess , IDLE_PRIORITY_CLASS);
	else if(priority=="NORMAL")
		SetPriorityClass(piArr[index-1].hProcess , NORMAL_PRIORITY_CLASS);
	else
		SetPriorityClass(piArr[index-1].hProcess , REALTIME_PRIORITY_CLASS);
	display(ui->infoPlainTextEdit,ui->comboBox->currentText().toInt());
}
