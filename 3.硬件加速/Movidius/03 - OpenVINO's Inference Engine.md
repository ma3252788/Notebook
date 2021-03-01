# 03 - OpenVINO's Inference Engine

## 一、OpenVINO 的使用实例

官方给出的 OpenVINO 的 API 的介绍在 [[OpenVINO-InferEngine]](https://software.intel.com/en-us/articles/OpenVINO-InferEngine)，其中介绍了 **IEPlugin()** 类和 **IENetwork()** 类，以及一些常用的使用方法，以下是我继上一个模型，使用自己编写的模型编译为 **OpenVINO IR** 模型并进行预测，在 **jupyter notebook** 中做的实验，以下是实验代码及其结果



## 二、测试代码

```python
python
import numpy as np
from openvino.inference_engine import IENetwork, IEPlugin
from PIL import Image
import time
```

```python
model_name = 'cnn_model'

device = 'MYRIAD'
# device = 'CPU'

file_dir = 'FP16' if device == 'MYRIAD' else 'FP32'

xml_path = '../{}/{}.ckpt.xml'.format(file_dir, model_name)
bin_path = '../{}/{}.ckpt.bin'.format(file_dir, model_name)

print('=' * 30)
print('device: {}'.format(device))
print('=' * 30)
print('xml_path: {}'.format(xml_path))
print('=' * 30)
print('bin_path: {}'.format(bin_path))
print('=' * 30)
```

```shell
==============================
device: MYRIAD
==============================
xml_path: ../FP16/cnn_model.ckpt.xml
==============================
bin_path: ../FP16/cnn_model.ckpt.bin
==============================
```

```python
# 初始化插入设备
plugin = IEPlugin(device=device)

# 载入网络模型
network = IENetwork(model=xml_path, weights=bin_path)

# 网络的 inputs 和 outputs 都是用 dict 保存的
print('=' * 30)
print(network.inputs)
print('=' * 30)
print(network.outputs)
print('=' * 30)
```

```shell
==============================
{'x-input': <openvino.inference_engine.ie_api.InputInfo object at 0x7f2f8d6dcb78>}
==============================
{'layer8-fc2/Softmax': <openvino.inference_engine.ie_api.OutputInfo object at 0x7f2f8d6fe5d0>}
==============================
```

```python
# 需要注意的是 OpenVINO 的图片输入格式是 N, C, H, W, 但是 tensorflow 一般使用格式为 N, H, W, C

# image_path = '../DATA/mnist_test/2/mnist_test_1016.png'
# image_path = '../DATA/mnist_test/3/mnist_test_1062.png'
image_path = '../DATA/mnist_test/4/mnist_test_1015.png'
# image_path = '../DATA/mnist_test/5/mnist_test_1003.png'

image_data = np.asarray(Image.open(image_path))
resize_image_data = np.zeros([1, image_data.shape[0], image_data.shape[1], 1], dtype=np.float32)
resize_image_data[0, :, :, 0] = image_data
resize_image_data = resize_image_data.transpose([0, 3, 1, 2])

print('=' * 30)
print('image_data.shape: {}'.format(image_data.shape))
print('=' * 30)
print('resize_image_data.shape: {}'.format(resize_image_data.shape))
print('=' * 30)
```

```shell
==============================
image_data.shape: (28, 28)
==============================
resize_image_data.shape: (1, 1, 28, 28)
==============================
```

```python
# 将模型载入设备
start_load_time = time.time()
exec_network = plugin.load(network)
end_load_time = time.time()

print('=' * 30)
print('load network to plugin cost: {:2.5f} ms'.format((end_load_time - start_load_time) * 1000))
print('=' * 30)
```

```shell
==============================
load network to plugin cost: 3194.89026 ms
==============================
```

```python
# 在这里, 只有一个输入, 一个输出
input_blob = next(iter(network.inputs))
output_blob = next(iter(network.outputs))


print('=' * 30)
print('input_blob: {}'.format(input_blob))
print('=' * 30)
print('output_blob: {}'.format(output_blob))
print('=' * 30)
```

```python
# 使用 infer 推断
infer_start_time = time.time()
infer_result = exec_network.infer(inputs={input_blob: resize_image_data})
infer_end_time = time.time()

output_result = infer_result[output_blob]

predict_num = np.argmax(output_result)

print('=' * 30)
print('infer process cost: {} ms'.format((infer_end_time - infer_start_time) * 1000))
print('=' * 30)
print('infer result: {}'.format(infer_result))
print('=' * 30)
print('output_result: {}'. format(output_result))
print('=' * 30)
print('predict_num: {}'.format(predict_num))
print('=' * 30)
```

```shell
==============================
infer process cost: 2.403736114501953 ms
==============================
infer result: {'layer8-fc2/Softmax': array([[0., 0., 0., 0., 1., 0., 0., 0., 0., 0.]], dtype=float32)}
==============================
output_result: [[0. 0. 0. 0. 1. 0. 0. 0. 0. 0.]]
==============================
predict_num: 4
==============================
```

