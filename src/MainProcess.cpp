#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

void ConvertColor(const cv::Mat &inmat, cv::Mat &outmat, int ccc) {
  cv::cvtColor(inmat, outmat, ccc);
  return;
}

const int pixel = 150;
const int offset = 10;
const int CNT = 9;
void CreateAruCoMarker() {
  cv::Mat img = cv::Mat::zeros(pixel + offset, pixel + offset, CV_8UC1);
  img += cv::Scalar(255, 255, 255);
  int x_offset = (int)(offset / 2);
  int y_offset = (int)(offset / 2);
  cv::Mat hconcat_img, vconcat_img;

  cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);

  for(int lpcti = 0; lpcti < CNT; lpcti++){
    cv::Mat ar_img;
    cv::aruco::generateImageMarker(dictionary, lpcti, pixel, ar_img, 1);
    /* ファイル名の設定 */
    std::string file_nm = "ar" + std::to_string(lpcti) + ".png";
    /* アフィン行列 */
    cv::Mat mat = (cv::Mat_<double>(2,3)<<1.0, 0.0, x_offset, 0.0, 1.0, y_offset);
    
    /* アフィン変換 */
    cv::warpAffine(ar_img, img, mat, img.size(), cv::INTER_LINEAR, cv::BORDER_TRANSPARENT);
    /* グレースケールからRGBへ変換 */
    cv::Mat rgb_img;
    cv::cvtColor(img, rgb_img, cv::COLOR_GRAY2RGB);

    if(lpcti % 3 == 0){
      hconcat_img = rgb_img;
    } else if(lpcti % 3 <=2){
      cv::hconcat(hconcat_img, rgb_img, hconcat_img);				
      if(lpcti % 3 == 2 && lpcti / 3 == 0){
        vconcat_img = hconcat_img;
      } else if(lpcti % 3 == 2 && lpcti / 3 > 0){
        cv::vconcat(vconcat_img, hconcat_img, vconcat_img);
      }
    }
    /* Output RGB image. */
    cv::imwrite(file_nm, rgb_img);
  }

  cv::imwrite("ar" + std::to_string(CNT) + ".png", vconcat_img);
  return;
}

void DetectAruCoMarker() {
  cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
  cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_100);
  cv::aruco::ArucoDetector detector(dictionary, detectorParams);

  for(int lpcti = 0; lpcti < CNT + 1; lpcti++){
    // 入力ファイル名
    std::string input_file_nm = "ar" + std::to_string(lpcti) + ".png";
    // 出力ファイル名
    std::string output_file_nm = "ar_detection" + std::to_string(lpcti) + ".png";
    // 入力ファイルの読み込み
    cv::Mat input_img = cv::imread(input_file_nm, cv::IMREAD_COLOR);

    std::vector<std::vector<cv::Point2f>> corners, rejectedCandidates;
    std::vector<int> ids;

    // ArUcoマーカの検出
    detector.detectMarkers(input_img, corners, ids, rejectedCandidates);

    if(ids.size() > 0){
      cv::Mat output_img = input_img.clone();
      // ArUcoマーカの検出結果の描画
      cv::aruco::drawDetectedMarkers(output_img, corners, ids);
      // ArUcoマーカの検出結果をファイル出力
      cv::imwrite(output_file_nm, output_img);
    }
  }
  return;
}
