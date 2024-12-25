#ifndef COMMUNICATIONPROTOCOL_H
#define COMMUNICATIONPROTOCOL_H

#include <QJsonObject>
#include <QString>
#include <QJsonArray>
#include <QList>

namespace CommunicationProtocol {

enum Action {
    LOGIN = 1,
    ADD_TEACHER,
    GET_TEACHERS,
    UPDATE_TEACHER,
    DELETE_TEACHER,
    ADD_STUDENT,
    GET_STUDENTS,
    UPDATE_STUDENT,
    DELETE_STUDENT,
    SEARCH_STUDENT,
    CREATE_EXAM,
    DELETE_EXAM,
    GET_EXAMS,
    UPLOAD_ANSWER_SHEETS,
    DOWNLOAD_ANSWER_SHEET,
    UPLOAD_GRADE,
    GET_SCORES,
    ERROR,
    SUCCESS
};

// 1 用户登录
struct LoginRequest {
    int action = LOGIN;
    QString username;
    QString password;
    QString userType;

    QJsonObject toJson() const { // 从字典转化为字节流
        QJsonObject request;
        request["action"] = action;
        request["username"] = username;
        request["password"] = password;
        request["user_type"] = userType;
        return request;
    }

    static LoginRequest fromJson(const QJsonObject &json) { // 从字节流转化为字典
        LoginRequest request;
        request.action = json["action"].toInt();
        request.username = json["username"].toString();
        request.password = json["password"].toString();
        request.userType = json["user_type"].toString();
        return request;
    }
};

struct LoginResponse {
    int action = LOGIN;
    bool success;
    QString userType;
    QString userId;
    QString errorMessage;

    static LoginResponse fromJson(const QJsonObject &json) {
        LoginResponse response;
        response.action = json["action"].toInt();
        response.success = json["status"].toString() == "success";
        if (response.success) {
            response.userType = json["user_type"].toString();
            response.userId = json["user_id"].toString();
        } else {
            response.errorMessage = json["error_message"].toString();
        }
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["status"] = success ? "success" : "failure";
        if (success) {
            response["user_type"] = userType;
            response["user_id"] = userId;
        } else {
            response["error_message"] = errorMessage;
        }
        return response;
    }
};

// 5 错误回复
struct ErrorResponse {
    int action = ERROR;
    QString errorMessage;

    static ErrorResponse fromJson(const QJsonObject &json) {
        ErrorResponse response;
        response.action = json["action"].toInt();
        response.errorMessage = json["error_message"].toString();
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["error_message"] = errorMessage;
        return response;
    }
};

// 6 成功回复
struct SuccessResponse {
    int action = SUCCESS;
    QString successMessage;

    static SuccessResponse fromJson(const QJsonObject &json) {
        SuccessResponse response;
        response.action = json["action"].toInt();
        response.successMessage = json["success_message"].toString();
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["success_message"] = successMessage;
        return response;
    }
};

// 添加教师
struct AddTeacherRequest {
    int action = ADD_TEACHER;
    QString name;
    QString account;
    QString password;

    QJsonObject toJson() const {
        QJsonObject request;
        request["action"] = action;
        request["name"] = name;
        request["account"] = account;
        request["password"] = password;
        return request;
    }

    static AddTeacherRequest fromJson(const QJsonObject &json) {
        AddTeacherRequest request;
        request.action = json["action"].toInt();
        request.name = json["name"].toString();
        request.account = json["account"].toString();
        request.password = json["password"].toString();
        return request;
    }
};

struct AddTeacherResponse {
    int action = ADD_TEACHER;
    bool success;
    QString errorMessage;

    static AddTeacherResponse fromJson(const QJsonObject &json) {
        AddTeacherResponse response;
        response.action = json["action"].toInt();
        response.success = json["status"].toString() == "success";
        if (!response.success) {
            response.errorMessage = json["error_message"].toString();
        }
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["status"] = success ? "success" : "failure";
        if (!success) {
            response["error_message"] = errorMessage;
        }
        return response;
    }
};

// 获取教师列表
struct GetTeachersRequest {
    int action = GET_TEACHERS;

    QJsonObject toJson() const {
        QJsonObject request;
        request["action"] = action;
        return request;
    }

    static GetTeachersRequest fromJson(const QJsonObject &json) {
        GetTeachersRequest request;
        request.action = json["action"].toInt();
        return request;
    }
};

struct GetTeachersResponse {
    int action = GET_TEACHERS;
    bool success;
    QList<QJsonObject> teachers;
    QString errorMessage;

    static GetTeachersResponse fromJson(const QJsonObject &json) {
        GetTeachersResponse response;
        response.action = json["action"].toInt();
        response.success = json["status"].toString() == "success";
        if (response.success) {
            QJsonArray teacherArray = json["teachers"].toArray();
            for (const auto &teacherValue : teacherArray) {
                response.teachers.append(teacherValue.toObject());
            }
        } else {
            response.errorMessage = json["error_message"].toString();
        }
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["status"] = success ? "success" : "failure";
        if (success) {
            QJsonArray teacherArray;
            for (const auto &teacher : teachers) {
                teacherArray.append(teacher);
            }
            response["teachers"] = teacherArray;
        } else {
            response["error_message"] = errorMessage;
        }
        return response;
    }
};

// 修改教师
struct UpdateTeacherRequest {
    int action = UPDATE_TEACHER;
    int teacherId;
    QString name;
    QString account;
    QString password;

    QJsonObject toJson() const {
        QJsonObject request;
        request["action"] = action;
        request["teacher_id"] = teacherId;
        request["name"] = name;
        request["account"] = account;
        request["password"] = password;
        return request;
    }

    static UpdateTeacherRequest fromJson(const QJsonObject &json) {
        UpdateTeacherRequest request;
        request.action = json["action"].toInt();
        request.teacherId = json["teacher_id"].toInt();
        request.name = json["name"].toString();
        request.account = json["account"].toString();
        request.password = json["password"].toString();
        return request;
    }
};

struct UpdateTeacherResponse {
    int action = UPDATE_TEACHER;
    bool success;
    QString errorMessage;

    static UpdateTeacherResponse fromJson(const QJsonObject &json) {
        UpdateTeacherResponse response;
        response.action = json["action"].toInt();
        response.success = json["status"].toString() == "success";
        if (!response.success) {
            response.errorMessage = json["error_message"].toString();
        }
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["status"] = success ? "success" : "failure";
        if (!success) {
            response["error_message"] = errorMessage;
        }
        return response;
    }
};

// 删除教师
struct DeleteTeacherRequest {
    int action = DELETE_TEACHER;
    int teacherId;

    QJsonObject toJson() const {
        QJsonObject request;
        request["action"] = action;
        request["teacher_id"] = teacherId;
        return request;
    }

    static DeleteTeacherRequest fromJson(const QJsonObject &json) {
        DeleteTeacherRequest request;
        request.action = json["action"].toInt();
        request.teacherId = json["teacher_id"].toInt();
        return request;
    }
};

struct DeleteTeacherResponse {
    int action = DELETE_TEACHER;
    bool success;
    QString errorMessage;

    static DeleteTeacherResponse fromJson(const QJsonObject &json) {
        DeleteTeacherResponse response;
        response.action = json["action"].toInt();
        response.success = json["status"].toString() == "success";
        if (!response.success) {
            response.errorMessage = json["error_message"].toString();
        }
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["status"] = success ? "success" : "failure";
        if (!success) {
            response["error_message"] = errorMessage;
        }
        return response;
    }
};

// 添加学生
struct AddStudentRequest {
    int action = ADD_STUDENT;
    QString name;
    QString studentId;
    QString major;
    QString className;
    QString password;

    QJsonObject toJson() const {
        QJsonObject request;
        request["action"] = action;
        request["name"] = name;
        request["student_id"] = studentId;
        request["major"] = major;
        request["class"] = className;
        request["password"] = password;
        return request;
    }

    static AddStudentRequest fromJson(const QJsonObject &json) {
        AddStudentRequest request;
        request.action = json["action"].toInt();
        request.name = json["name"].toString();
        request.studentId = json["student_id"].toString();
        request.major = json["major"].toString();
        request.className = json["class"].toString();
        request.password = json["password"].toString();
        return request;
    }
};

struct AddStudentResponse {
    int action = ADD_STUDENT;
    bool success;
    QString errorMessage;

    static AddStudentResponse fromJson(const QJsonObject &json) {
        AddStudentResponse response;
        response.action = json["action"].toInt();
        response.success = json["status"].toString() == "success";
        if (!response.success) {
            response.errorMessage = json["error_message"].toString();
        }
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["status"] = success ? "success" : "failure";
        if (!success) {
            response["error_message"] = errorMessage;
        }
        return response;
    }
};

// 获取学生列表
struct GetStudentsRequest {
    int action = GET_STUDENTS;

    QJsonObject toJson() const {
        QJsonObject request;
        request["action"] = action;
        return request;
    }

    static GetStudentsRequest fromJson(const QJsonObject &json) {
        GetStudentsRequest request;
        request.action = json["action"].toInt();
        return request;
    }
};

struct GetStudentsResponse {
    int action = GET_STUDENTS;
    bool success;
    QList<QJsonObject> students;
    QString errorMessage;

    static GetStudentsResponse fromJson(const QJsonObject &json) {
        GetStudentsResponse response;
        response.action = json["action"].toInt();
        response.success = json["status"].toString() == "success";
        if (response.success) {
            QJsonArray studentArray = json["students"].toArray();
            for (const auto &studentValue : studentArray) {
                response.students.append(studentValue.toObject());
            }
        } else {
            response.errorMessage = json["error_message"].toString();
        }
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["status"] = success ? "success" : "failure";
        if (success) {
            QJsonArray studentArray;
            for (const auto &student : students) {
                studentArray.append(student);
            }
            response["students"] = studentArray;
        } else {
            response["error_message"] = errorMessage;
        }
        return response;
    }
};

// 修改学生
struct UpdateStudentRequest {
    int action = UPDATE_STUDENT;
    int studentId;
    QString name;
    QString studentIdNew;
    QString major;
    QString className;
    QString password;

    QJsonObject toJson() const {
        QJsonObject request;
        request["action"] = action;
        request["student_id"] = studentId;
        request["name"] = name;
        request["student_id_new"] = studentIdNew;
        request["major"] = major;
        request["class"] = className;
        request["password"] = password;
        return request;
    }

    static UpdateStudentRequest fromJson(const QJsonObject &json) {
        UpdateStudentRequest request;
        request.action = json["action"].toInt();
        request.studentId = json["student_id"].toInt();
        request.name = json["name"].toString();
        request.studentIdNew = json["student_id_new"].toString();
        request.major = json["major"].toString();
        request.className = json["class"].toString();
        request.password = json["password"].toString();
        return request;
    }
};

struct UpdateStudentResponse {
    int action = UPDATE_STUDENT;
    bool success;
    QString errorMessage;

    static UpdateStudentResponse fromJson(const QJsonObject &json) {
        UpdateStudentResponse response;
        response.action = json["action"].toInt();
        response.success = json["status"].toString() == "success";
        if (!response.success) {
            response.errorMessage = json["error_message"].toString();
        }
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["status"] = success ? "success" : "failure";
        if (!success) {
            response["error_message"] = errorMessage;
        }
        return response;
    }
};

// 删除学生
struct DeleteStudentRequest {
    int action = DELETE_STUDENT;
    int studentId;

    QJsonObject toJson() const {
        QJsonObject request;
        request["action"] = action;
        request["student_id"] = studentId;
        return request;
    }

    static DeleteStudentRequest fromJson(const QJsonObject &json) {
        DeleteStudentRequest request;
        request.action = json["action"].toInt();
        request.studentId = json["student_id"].toInt();
        return request;
    }
};

struct DeleteStudentResponse {
    int action = DELETE_STUDENT;
    bool success;
    QString errorMessage;

    static DeleteStudentResponse fromJson(const QJsonObject &json) {
        DeleteStudentResponse response;
        response.action = json["action"].toInt();
        response.success = json["status"].toString() == "success";
        if (!response.success) {
            response.errorMessage = json["error_message"].toString();
        }
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["status"] = success ? "success" : "failure";
        if (!success) {
            response["error_message"] = errorMessage;
        }
        return response;
    }
};

// 7 搜索学生
struct SearchStudentRequest {
    int action = SEARCH_STUDENT;
    QString name;
    QString className;
    QString major;
    QString number;

    QJsonObject toJson() const {
        QJsonObject request;
        request["action"] = action;
        request["name"] = name;
        request["className"] = className;
        request["major"] = major;
        request["number"] = number;
        return request;
    }

    static SearchStudentRequest fromJson(const QJsonObject &json) {
        SearchStudentRequest request;
        request.action = json["action"].toInt();
        request.name = json["name"].toString();
        request.className = json["className"].toString();
        request.major = json["major"].toString();
        request.number = json["number"].toString();
        return request;
    }
};

struct SearchStudentResponse {
    int action = SEARCH_STUDENT;
    bool success;
    QList<QJsonObject> students;
    QString errorMessage;

    static SearchStudentResponse fromJson(const QJsonObject &json) {
        SearchStudentResponse response;
        response.action = json["action"].toInt();
        response.success = json["status"].toString() == "success";
        if (response.success) {
            QJsonArray studentArray = json["students"].toArray();
            for (const auto &studentValue : studentArray) {
                response.students.append(studentValue.toObject());
            }
        } else {
            response.errorMessage = json["error_message"].toString();
        }
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["status"] = success ? "success" : "failure";
        if (success) {
            QJsonArray studentArray;
            for (const auto &student : students) {
                studentArray.append(student);
            }
            response["students"] = studentArray;
        } else {
            response["error_message"] = errorMessage;
        }
        return response;
    }
};

// 3 发表考试
struct CreateExamRequest {
    int action = CREATE_EXAM;
    QString academicYear;
    QString semester;
    QString course;
    QString teacher;
    QString data;
    QString end;

    QJsonObject toJson() const {
        QJsonObject request;
        request["action"] = action;
        request["academic_year"] = academicYear;
        request["semester"] = semester;
        request["course"] = course;
        request["teacher"] = teacher;
        request["data"] = data;
        request["end"] = end;
        return request;
    }

    static CreateExamRequest fromJson(const QJsonObject &json) {
        CreateExamRequest request;
        request.action = json["action"].toInt();
        request.academicYear = json["academic_year"].toString();
        request.semester = json["semester"].toString();
        request.course = json["course"].toString();
        request.teacher = json["teacher"].toString();
        request.data = json["data"].toString();
        request.end = json["end"].toString();
        return request;
    }
};

struct CreateExamResponse {
    int action = CREATE_EXAM;
    bool success;
    QString errorMessage;

    static CreateExamResponse fromJson(const QJsonObject &json) {
        CreateExamResponse response;
        response.action = json["action"].toInt();
        response.success = json["status"].toString() == "success";
        if (!response.success) {
            response.errorMessage = json["error_message"].toString();
        }
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["status"] = success ? "success" : "failure";
        if (!success) {
            response["error_message"] = errorMessage;
        }
        return response;
    }
};

// 4 删除考试
struct DeleteExamRequest {
    int action = DELETE_EXAM;
    QString academicYear;
    QString semester;
    QString course;
    QString teacher;
    QString data;
    QString end;

    QJsonObject toJson() const {
        QJsonObject request;
        request["action"] = action;
        request["academic_year"] = academicYear;
        request["semester"] = semester;
        request["course"] = course;
        request["teacher"] = teacher;
        request["data"] = data;
        request["end"] = end;
        return request;
    }

    static DeleteExamRequest fromJson(const QJsonObject &json) {
        DeleteExamRequest request;
        request.action = json["action"].toInt();
        request.academicYear = json["academic_year"].toString();
        request.semester = json["semester"].toString();
        request.course = json["course"].toString();
        request.teacher = json["teacher"].toString();
        request.data = json["data"].toString();
        request.end = json["end"].toString();
        return request;
    }
};

struct DeleteExamResponse {
    int action = DELETE_EXAM;
    bool success;
    QString errorMessage;

    static DeleteExamResponse fromJson(const QJsonObject &json) {
        DeleteExamResponse response;
        response.action = json["action"].toInt();
        response.success = json["status"].toString() == "success";
        if (!response.success) {
            response.errorMessage = json["error_message"].toString();
        }
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["status"] = success ? "success" : "failure";
        if (!success) {
            response["error_message"] = errorMessage;
        }
        return response;
    }
};


// 2 获取考试列表
struct GetExamsRequest {
    int action = GET_EXAMS;
    QString academicYear;
    QString semester;
    QString course;
    QString teacher;
    QString data;
    QString end;

    QJsonObject toJson() const {
        QJsonObject request;
        request["action"] = action;
        request["academic_year"] = academicYear;
        request["semester"] = semester;
        request["course"] = course;
        request["teacher"] = teacher;
        request["data"] = data;
        request["end"] = end;
        return request;
    }

    static GetExamsRequest fromJson(const QJsonObject &json) {
        GetExamsRequest request;
        request.action = json["action"].toInt();
        request.academicYear = json["academic_year"].toString();
        request.semester = json["semester"].toString();
        request.course = json["course"].toString();
        request.teacher = json["teacher"].toString();
        request.data = json["data"].toString();
        request.end = json["end"].toString();
        return request;
    }
};

struct GetExamsResponse {
    int action = GET_EXAMS;
    bool success;
    QList<QJsonObject> exams;
    QString errorMessage;

    static GetExamsResponse fromJson(const QJsonObject &json) {
        GetExamsResponse response;
        response.action = json["action"].toInt();
        response.success = json["status"].toString() == "success";
        if (response.success) {
            QJsonArray examArray = json["exams"].toArray();
            for (const auto &examValue : examArray) {
                response.exams.append(examValue.toObject());
            }
        } else {
            response.errorMessage = json["error_message"].toString();
        }
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["status"] = success ? "success" : "failure";
        if (success) {
            QJsonArray examArray;
            for (const auto &exam : exams) {
                examArray.append(exam);
            }
            response["exams"] = examArray;
        } else {
            response["error_message"] = errorMessage;
        }
        return response;
    }
};

// 批量上传答题卡图片
struct UploadAnswerSheetsRequest {
    int action = UPLOAD_ANSWER_SHEETS;
    int examId;
    QList<QPair<QString, QString>> images; // image_id, base64_data

    QJsonObject toJson() const {
        QJsonObject request;
        request["action"] = action;
        request["exam_id"] = examId;
        QJsonArray imagesArray;
        for (const auto &image : images) {
            QJsonObject imageObj;
            imageObj["image_id"] = image.first;
            imageObj["data"] = image.second;
            imagesArray.append(imageObj);
        }
        request["images"] = imagesArray;
        return request;
    }

    static UploadAnswerSheetsRequest fromJson(const QJsonObject &json) {
        UploadAnswerSheetsRequest request;
        request.action = json["action"].toInt();
        request.examId = json["exam_id"].toInt();
        QJsonArray imagesArray = json["images"].toArray();
        for (const auto &imageValue : imagesArray) {
            QJsonObject imageObj = imageValue.toObject();
            request.images.append(qMakePair(imageObj["image_id"].toString(), imageObj["data"].toString()));
        }
        return request;
    }
};

struct UploadAnswerSheetsResponse {
    int action = UPLOAD_ANSWER_SHEETS;
    bool success;
    QString errorMessage;

    static UploadAnswerSheetsResponse fromJson(const QJsonObject &json) {
        UploadAnswerSheetsResponse response;
        response.action = json["action"].toInt();
        response.success = json["status"].toString() == "success";
        if (!response.success) {
            response.errorMessage = json["error_message"].toString();
        }
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["status"] = success ? "success" : "failure";
        if (!success) {
            response["error_message"] = errorMessage;
        }
        return response;
    }
};

// 下载答题卡图片
struct DownloadAnswerSheetRequest {
    int action = DOWNLOAD_ANSWER_SHEET;
    QString answerSheetId;

    QJsonObject toJson() const {
        QJsonObject request;
        request["action"] = action;
        request["answer_sheet_id"] = answerSheetId;
        return request;
    }

    static DownloadAnswerSheetRequest fromJson(const QJsonObject &json) {
        DownloadAnswerSheetRequest request;
        request.action = json["action"].toInt();
        request.answerSheetId = json["answer_sheet_id"].toString();
        return request;
    }
};

struct DownloadAnswerSheetResponse {
    int action = DOWNLOAD_ANSWER_SHEET;
    bool success;
    QString imageData;
    QString errorMessage;

    static DownloadAnswerSheetResponse fromJson(const QJsonObject &json) {
        DownloadAnswerSheetResponse response;
        response.action = json["action"].toInt();
        response.success = json["status"].toString() == "success";
        if (response.success) {
            response.imageData = json["data"].toString();
        } else {
            response.errorMessage = json["error_message"].toString();
        }
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["status"] = success ? "success" : "failure";
        if (success) {
            response["data"] = imageData;
        } else {
            response["error_message"] = errorMessage;
        }
        return response;
    }
};

// 上传评分信息
struct UploadGradeRequest {
    int action = UPLOAD_GRADE;
    QString answerSheetId;
    int score;

    QJsonObject toJson() const {
        QJsonObject request;
        request["action"] = action;
        request["answer_sheet_id"] = answerSheetId;
        request["score"] = score;
        return request;
    }

    static UploadGradeRequest fromJson(const QJsonObject &json) {
        UploadGradeRequest request;
        request.action = json["action"].toInt();
        request.answerSheetId = json["answer_sheet_id"].toString();
        request.score = json["score"].toInt();
        return request;
    }
};

struct UploadGradeResponse {
    int action = UPLOAD_GRADE;
    bool success;
    QString errorMessage;

    static UploadGradeResponse fromJson(const QJsonObject &json) {
        UploadGradeResponse response;
        response.action = json["action"].toInt();
        response.success = json["status"].toString() == "success";
        if (!response.success) {
            response.errorMessage = json["error_message"].toString();
        }
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["status"] = success ? "success" : "failure";
        if (!success) {
            response["error_message"] = errorMessage;
        }
        return response;
    }
};

// 8 获取成绩
struct GetScoresRequest {
    int action = GET_SCORES;
    QString name;
    QString className;
    QString major;
    QString number;
    QString course;

    QJsonObject toJson() const {
        QJsonObject request;
        request["action"] = action;
        request["name"] = name;
        request["className"] = className;
        request["major"] = major;
        request["number"] = number;
        request["course"] = course;
        return request;
    }

    static GetScoresRequest fromJson(const QJsonObject &json) {
        GetScoresRequest request;
        request.action = json["action"].toInt();
        request.name = json["name"].toString();
        request.className = json["className"].toString();
        request.major = json["major"].toString();
        request.number = json["number"].toString();
        request.course = json["course"].toString();
        return request;
    }
};

struct GetScoresResponse {
    int action = GET_SCORES;
    bool success;
    QList<QJsonObject> scores;
    QString errorMessage;

    static GetScoresResponse fromJson(const QJsonObject &json) {
        GetScoresResponse response;
        response.action = json["action"].toInt();
        response.success = json["status"].toString() == "success";
        if (response.success) {
            QJsonArray scoreArray = json["scores"].toArray();
            for (const auto &scoreValue : scoreArray) {
                response.scores.append(scoreValue.toObject());
            }
        } else {
            response.errorMessage = json["error_message"].toString();
        }
        return response;
    }

    QJsonObject toJson() const {
        QJsonObject response;
        response["action"] = action;
        response["status"] = success ? "success" : "failure";
        if (success) {
            QJsonArray scoreArray;
            for (const auto &score : scores) {
                scoreArray.append(score);
            }
            response["scores"] = scoreArray;
        } else {
            response["error_message"] = errorMessage;
        }
        return response;
    }
};

} // namespace CommunicationProtocol

#endif // COMMUNICATIONPROTOCOL_H



