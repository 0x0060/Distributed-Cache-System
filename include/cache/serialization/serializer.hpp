#pragma once

#include <string>
#include <vector>
#include <type_traits>

namespace cache::serialization {

template<typename T>
class Serializer {
public:
    virtual ~Serializer() = default;
    virtual std::vector<uint8_t> serialize(const T& value) = 0;
    virtual T deserialize(const std::vector<uint8_t>& data) = 0;
};

template<typename T>
class JsonSerializer : public Serializer<T> {
public:
    std::vector<uint8_t> serialize(const T& value) override {
        nlohmann::json json_obj = value.to_json();
        std::string json_str = json_obj.dump();
        return std::vector<uint8_t>(json_str.begin(), json_str.end());
    }

    T deserialize(const std::vector<uint8_t>& data) override {
        std::string json_str(data.begin(), data.end());
        nlohmann::json json_obj = nlohmann::json::parse(json_str);
        return T::from_json(json_obj);
    }
};

template<typename T>
class ProtobufSerializer : public Serializer<T> {
public:
    std::vector<uint8_t> serialize(const T& value) override {
        std::string serialized;
        if (!value.SerializeToString(&serialized)) {
            throw std::runtime_error("Failed to serialize protobuf message");
        }
        return std::vector<uint8_t>(serialized.begin(), serialized.end());
    }

    T deserialize(const std::vector<uint8_t>& data) override {
        T value;
        std::string serialized(data.begin(), data.end());
        if (!value.ParseFromString(serialized)) {
            throw std::runtime_error("Failed to deserialize protobuf message");
        }
        return value;
    }
};

} // namespace cache::serialization