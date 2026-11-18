#include <iostream>
#include <cstdint>
#include <algorithm>
#include <type_traits>

struct CameraFrame {
    int w, h;
    const unsigned char* buf;
};

struct RosImage {
    int w, h, step;
    const unsigned char* data;
};

struct VideoFrame {
    int w, h;
    const unsigned char* yuv;
};

namespace virt {

class ImageSource {
public:
    virtual ~ImageSource() = default;
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual unsigned char at(int x, int y) const = 0;
};

class CameraSource : public ImageSource {
private:
    CameraFrame frame_;

public:
    explicit CameraSource(CameraFrame frame) : frame_(frame) {}

    int width() const override { return frame_.w; }
    int height() const override { return frame_.h; }

    unsigned char at(int x, int y) const override {
        return frame_.buf[y * frame_.w + x];
    }
};

class RosSource : public ImageSource {
private:
    RosImage image_;

public:
    explicit RosSource(RosImage image) : image_(image) {}

    int width() const override { return image_.w; }
    int height() const override { return image_.h; }

    unsigned char at(int x, int y) const override {
        return image_.data[y * image_.step + x];
    }
};

class VideoSource : public ImageSource {
private:
    VideoFrame frame_;

public:
    explicit VideoSource(VideoFrame frame) : frame_(frame) {}

    int width() const override { return frame_.w; }
    int height() const override { return frame_.h; }

    unsigned char at(int x, int y) const override {
        int yValue = frame_.yuv[y * frame_.w + x];
        return static_cast<unsigned char>(std::max(yValue - 16, 0));
    }
};

std::uint64_t preprocess(const ImageSource& src) {
    std::uint64_t sum = 0;
    for (int y = 0; y < src.height(); ++y) {
        for (int x = 0; x < src.width(); ++x) {
            sum += src.at(x, y);
        }
    }
    return sum;
}

}  // namespace virt

namespace tmpl {

template <typename Frame>
std::uint64_t preprocess(const Frame& frame) {
    std::uint64_t sum = 0;
    for (int y = 0; y < frame.h; ++y) {
        for (int x = 0; x < frame.w; ++x) {
            if constexpr (std::is_same_v<Frame, CameraFrame>) {
                sum += frame.buf[y * frame.w + x];
            } else if constexpr (std::is_same_v<Frame, RosImage>) {
                sum += frame.data[y * frame.step + x];
            } else {
                static_assert(std::is_same_v<Frame, VideoFrame>, "Unsupported frame type");
                int yValue = frame.yuv[y * frame.w + x];
                sum += std::max(yValue - 16, 0);
            }
        }
    }
    return sum;
}

}  // namespace tmpl

int main() {
    const unsigned char pixels[] = {10, 20, 30, 40, 50, 60};
    CameraFrame frame{3, 2, pixels};
    virt::CameraSource camera(frame);
    std::cout << virt::preprocess(camera) << ' ' << tmpl::preprocess(frame) << '\n';

    const unsigned char rosPixels[] = {10, 20, 99, 30, 40, 99};
    RosImage rosFrame{2, 2, 3, rosPixels};
    virt::RosSource ros(rosFrame);
    std::cout << virt::preprocess(ros) << ' ' << tmpl::preprocess(rosFrame) << '\n';

    const unsigned char videoPixels[] = {10, 26, 235, 250, 99, 99};
    VideoFrame videoFrame{2, 2, videoPixels};
    virt::VideoSource video(videoFrame);
    std::cout << virt::preprocess(video) << ' ' << tmpl::preprocess(videoFrame) << '\n';
}