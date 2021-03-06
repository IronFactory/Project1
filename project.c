#include <stdio.h> 
#include <stdlib.h>
#include <time.h>

// 월별 일 수
int date[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
char stack[100];

// 스택 데이터 개수
int count;

// 스케줄 저장 배열
int mScheduleYear[100] = {0};
int mScheduleMonth[100] = {0};
int mScheduleDay[100] = {0};	
char mSchedule[100][20] = {'\0'};

// 스케줄 갯수
int mScheduleCount = 0;

// 스택 함수
void push(char);
void pushStr(char*);
char pop();
int isEmpty();

// 메뉴 함수
void scheduler();
void calculator();

// 계산기 함수
char getVariable(char*);
int getValue(char*);
int getLength(char*);

// 스케줄관리 함수
int checkDate(int, int, int);
int checkDay(int, int, int);
int checkMonth(int);
int schedulerMenu();
int getLeapYear(int, int);
void printCalendar(int, int, int, int);
int isLeapYear(int);
void insertSchedule(int);

// String 함수
char* removeEnterInFgetsString(char*);

int main(void) {
	int menuChoice;
	int subMenu;

	printf("\tMenu\n");
	printf("1. Calculator\n");
	printf("2. Scheduler\n");
	printf("3. End\n");

	scanf("%d", &menuChoice);
	switch(menuChoice) {
		case 1:
			// 계산기
			calculator();
			break;

		case 2:
			while (1) {
				// 스케쥴러
				subMenu = schedulerMenu();
				switch(subMenu) {
					case 1:
						insertSchedule(mScheduleCount);
						break;
				}
			}
			break;

		case 3:
			// 종료
			printf("종료됩니다.\n");
			break;
	}
	return 0;
}

// 스케쥴 관리 메뉴 출력
int schedulerMenu() {
	int menu;
	printf("1. 스케줄 입력\n");
	printf("2. 스케줄 삭제\n");
	printf("3. 스케줄 보기\n");
	printf("4. Main menu\n");
	printf("메뉴 : ");
	scanf("%d", &menu);
	return menu;
}


// fgets는 마지막에 \n도 저장되기에 \n을 제거.
char* removeEnterInFgetsString(char *str) {
	int length = getLength(str);
	str[length - 1] = '\0';
	return str;
}


// 스케줄 입력
void insertSchedule(int scheduleCount) {
	int year, month, day;
	char schedule[100];
	while (1) {
		printf("입력 : ");
		scanf("%d %d %d", &year, &month, &day);
		getchar();
		fgets(schedule, sizeof(schedule), stdin);
		removeEnterInFgetsString(schedule);

		// 날짜 제대로 입력했는지 확인
		if (checkDate(year, month, day)) {
			break;
		}
	}
	printf("출력 : %d년 %d월 %d일 %s\n", year, month, day, schedule);

	// 일정 겹치는지 확인
	int overwrite = 1;
	for (int i = 0; i < scheduleCount; i++) {
		// 겹침
		if (mScheduleYear[i] == year && mScheduleMonth[i] == month && mScheduleDay[i] == day) {
			char answer;
			printf("해당 날짜에 이미 일정이 있습니다. 덮어 씌우시겠습니까? (Y or N)");
			getchar();
			answer = getchar();
			// 덮어씌우기
			if (answer == 'Y' || answer == 'y') {
				scheduleCount = i;
				overwrite = 1;
			}
			// 되돌아가기
			else
				overwrite = 0;
			break;
		}
	}

	// 덮어쓰지 않겠다면 처음으로 돌아감
	if (!overwrite)
		return;

	printf("count = %d\n", scheduleCount);
	mScheduleYear[scheduleCount] = year;
	mScheduleMonth[scheduleCount] = month;
	mScheduleDay[scheduleCount] = day;
	if (mScheduleCount == scheduleCount)
		mScheduleCount++;

	printf("일정을 추가하였습니다.\n");
	printf("아무키나 입력하세요.......");
	getchar();
	system("clear");
}

int checkDate(int year, int month, int day) {
	// 1 ~ 12월 입력했는지 확인
	if (!checkMonth(month)) {
		printf("1 ~ 12월로 입력해주세요.\n");
		return 0;
	} else if (!checkDay(year, month, day)) {
		printf("%d년 %d월 %d일은 존재하지 않습니다.\n", year, month, day);
		return 0;
	}
	return 1;
}


// 월 제대로 입력했는지 확인
int checkMonth(int month) {
	if (month >= 1 && month <= 12)
		return 1;
	return 0;
}


// 일 제대로 입력했는지 확인
int checkDay(int year, int month, int day) {
	int leap = isLeapYear(year);
	if (day > 0 && day <= date[month - 1])
		return 1;
	else if (month == 2 && leap)
		if (day > 0 && day <= 29)
			return 1;
	return 0;
}


// 일정관리 앱
void scheduler() {
	// 현재 시간 달력 띄우기
	time_t nowTime = time(NULL);
	struct tm *t;
	t = localtime(&nowTime);

	int leapYear;
	int year = t->tm_year + 1900;
	int month = t->tm_mon + 1;
	int day = t->tm_mday;

	printf("%d년 %d월 %d일\n\n", year, month, day);

	leapYear = getLeapYear(year, month);
	printCalendar(year, leapYear, month, day);


	while (1) {
		int scheduleCount = mScheduleCount;
		printf("년도를 입력하세요 : ");
		scanf("%d", &year);
		// 월 제대로 입력했는지 확인
		while(1) {
			printf("월을 입력하세요 : ");
			scanf("%d", &month);
			if (month >= 1 && month <= 12)
				break;
			else {
				printf("다시 입력해주세요.\n");
			}
		}
	
		//일 제대로 입력했는지 확인
		while(1) {
			printf("일을 입력하세요 : ");
			scanf("%d", &day);
			if (day >= 1 && day <= date[month - 1])
				break;
			else {
				if (month == 2) {
					if(isLeapYear(year))
						if (day == 29)
							break;
				}
				printf("다시 입력해주세요.\n");
			}
		}
		leapYear = getLeapYear(year, month);
		// 캘린더 출력
		printCalendar(year, leapYear, month, day);
		
		// 일정 겹치는지 확인
		int overwrite = 1;
		for (int i = 0; i < scheduleCount; i++) {
			// 겹침
			if (mScheduleYear[i] == year && mScheduleMonth[i] == month && mScheduleDay[i] == day) {
				char answer;
				printf("해당 날짜에 이미 일정이 있습니다. 덮어 씌우시겠습니까? (Y or N)");
				getchar();
				answer = getchar();
				// 덮어씌우기
				if (answer == 'Y' || answer == 'y') {
					scheduleCount = i;
					overwrite = 1;
				}
				// 되돌아가기
				else
					overwrite = 0;
				break;
			}
		}

		// 일정 겹쳐서 되돌아가기
		if (overwrite == 0)
			continue;

		printf("일정을 입력하세요 : ");
		getchar();
		
		// 일정 입력
		fgets(mSchedule[scheduleCount], sizeof(mSchedule[scheduleCount]), stdin);
		
		// fgets는 마지막에 \n도 저장되기에 \n을 제거.
		int length = getLength(mSchedule[scheduleCount]);
		mSchedule[scheduleCount][length - 1] = '\0';

		// 일정 저장
		mScheduleYear[scheduleCount] = year;
		mScheduleMonth[scheduleCount] = month;
		mScheduleDay[scheduleCount] = day;
		mScheduleCount++;

		// 캘린더 출력
		printCalendar(year, leapYear, month, day);
	}
}


/*
 * 캘린더 출력
 * @Params : year = 년도
 *			leapYear = 윤년 횟수
 * 			month = 월
 *			day = 일
 */
void printCalendar(int year, int leapYear, int month, int day) {	
	// 1년1월1일로부터 해당 년월일까지 총 일 수 계산
	int allDay = year * 365 + leapYear;
	for (int i = 0; i < month - 1; i++)
		allDay += date[i];
	
	// 캘린더 출력 시 시작 요일 계산
	int skipDay = allDay % 7;
	
	// 2월이면 윤년인지 확인해서 윤년이면 29일까지 출력
	int isLeap = 0;
	if (month == 2 && isLeapYear(year))
		isLeap = 1;

	// 캘린더 출력 (윤년의 2월이면 29일까지, 시작 요일 포함)
	int weekCount = 0;
	printf("일\t\t월\t\t화\t\t수\t\t목\t\t금\t\t토\n");
	for (int i = 1; i <= date[month - 1] + isLeap + skipDay; i++, weekCount++) {
		// 시작 요일 맞추기 위해 건너뜀
		if (i <= skipDay)
			printf("\t\t");

		// 날짜 출력
		else
			printf("%d\t\t", i - skipDay);

		// 토요일까지 모두 출력 후 또는 day 모두 출력 후 개행
		if (i % 7 == 0 || i == date[month - 1] + isLeap + skipDay) {
			printf("\n");
			// 날짜 밑에 일정 출력
			for (int k = i - weekCount + 1; k <= i; k++) {
				weekCount = 0;
				// 해당 날짜에 일정 있는지 검사
				for (int j = 0; j < mScheduleCount; j++) {
					// 일정 있으면 출력
					if (mScheduleYear[j] == year && mScheduleMonth[j] == month && mScheduleDay[j] == k) {
						printf("%s", mSchedule[j]);
						break;
					}
				}
				printf("\t\t");
			}
			printf("\n");
		}
	}
	printf("\n\n");
}


// 윤년인지 확인
int isLeapYear(int year) {
	// 100으로 나누어 떨어지면 윤년아님
	if (year % 100 == 0) {
		// 그중에서 400으로 나누어 떨어지면 윤년임
		if (year % 400 == 0)
			return 1;
	} else if (year % 4 == 0)
		return 1;
	return 0;
}


// 윤년 몇 번 지나왔는지 계산
int getLeapYear(int year, int month) {
	int leapYear = 0;
	int includeYear = 0;
	// 2월 지났으면 해당 년도도 포함해서 계산
	if (month > 2)
		includeYear = 1;
	for (int i = 1; i < year + includeYear; i++) {
		if (isLeapYear(i))
			leapYear++;
	}
	return leapYear;
}


// 계산기
void calculator() {
	// 변수 지정 or 수학 식 임시 저장
	char str[100];

	// 변수 명 저장
	char variable[10];

	// 변수 값 저장
	int value[10];

	int variableCount = 0;

	// 연산자 임시 저장
	char operator;
	
	getchar();

	// 변수 지정
	while(1) {
		printf("변수 입력 : ");
		gets(str);
		if (getLength(str) == 0)
			break;
		variable[variableCount] = getVariable(str);
		value[variableCount] = getValue(str);
		printf("variable = %c\n", variable[variableCount]);
		printf("value = %d\n", value[variableCount]);
		variableCount++;
	}

	// 연산식 입력
	printf("연산식 입력 : ");
	gets(str);
	printf("연산식 = %s\n", str);

	// 
}


// 중위표기법 -> 후위 표기법
void convertToLast() {

}


// 문자열 길이 얻는 함수
int getLength(char *str) {
	int count = 0;
	while(*str++ != '\0')
		count++;
	return count;
}


/*
 * 문자열 중 변수 추출
 * Input : A = 10
 * Output : A
 */
char getVariable(char *str) {
	while(*str == ' ')
		str++;
	return *str;
}


/*
 * 문자열 중 변수 값 추출
 * Input : A = 10
 * Output : 10
 */
int getValue(char *str) {
	int result = 0;
	int count = 1;
	while(*str < '0' || *str > '9')
		str++;
	while(*str >= '0' && *str <= '9') {
		push(*str++);
	}
	while(!isEmpty()) {
		char c = pop();
		printf("%c\n", c);
		switch(c) {
			case '1':
				result += 1 * count;
				break;

			case '2':
				result += 2 * count;
				break;

			case '3':
				result += 3 * count;
				break;

			case '4':
				result += 4 * count;
				break;

			case '5':
				result += 5 * count;
				break;

			case '6':
				result += 6 * count;
				break;

			case '7':
				result += 7 * count;
				break;

			case '8':
				result += 8 * count;
				break;

			case '9':
				result += 9 * count;
				break;

		}
		count *= 10;
	}
	return result;
}


// 스택에 문자 하나 추가
void push(char chr) {
	stack[count++] = chr;
}


// 스택에 문자열 추가
void pushStr(char *str) {
	while(*str != '\0') {
		stack[count++] = *str++;
	}
}


// 스택에 있는 문자 하나 출력
char pop() {
	return stack[--count];
}


// 스택이 비었는지 확인
int isEmpty() {
	if (count == 0)
		return 1;
	return 0;
}
