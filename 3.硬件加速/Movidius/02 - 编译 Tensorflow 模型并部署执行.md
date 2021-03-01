## 02 - 编译TF模型并部署执行



## 一、使用 tensorflow 实现一个 CNN 卷积神经网络模型

* 先自己写一个 CNN 卷积神经网络模型，在训练完成后将模型保存在 `./model` 文件夹下

  <div align="center">
      <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210301152713320.png" width=700>
  </div>

* 在生成模型之后，保存为 **cnn_model.ckpt**，之后可以发现在 ./model 文件夹下多了四个文件，**这四个文件后面在做模型转换时需要**。

  <div align="center">
      <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210301152814102.png" width=700>
  </div>

## 二、编译为 OpenVINO 支持的模型

### 2.1 mo_tf.py 参数说明及注意要点

* 该`mo_tf.py`的路径为`/opt/intel/computer_vision_sdk/deployment_tools/model_optimizer/mo_tf.py`，官方参数说明如下：

  ```shell
  Framework-agnostic parameters:
    --input_model INPUT_MODEL, -w INPUT_MODEL, -m INPUT_MODEL
                          Tensorflow*: a file with a pre-trained model (binary
                          or text .pb file after freezing). Caffe*: a model
                          proto file with model weights
    --model_name MODEL_NAME, -n MODEL_NAME
                          Model_name parameter passed to the final create_ir
                          transform. This parameter is used to name a network in
                          a generated IR and output .xml/.bin files.
    --output_dir OUTPUT_DIR, -o OUTPUT_DIR
                          Directory that stores the generated IR. By default, it
                          is the directory from where the Model Optimizer is
                          launched.
    --input_shape INPUT_SHAPE
                          Input shape(s) that should be fed to an input node(s)
                          of the model. Shape is defined as a comma-separated
                          list of integer numbers enclosed in parentheses or
                          square brackets, for example [1,3,227,227] or
                          (1,227,227,3), where the order of dimensions depends
                          on the framework input layout of the model. For
                          example, [N,C,H,W] is used for Caffe* models and
                          [N,H,W,C] for TensorFlow* models. Model Optimizer
                          performs necessary transformations to convert the
                          shape to the layout required by Inference Engine
                          (N,C,H,W). The shape should not contain undefined
                          dimensions (? or -1) and should fit the dimensions
                          defined in the input operation of the graph. If there
                          are multiple inputs in the model, --input_shape should
                          contain definition of shape for each input separated
                          by a comma, for example: [1,3,227,227],[2,4] for a
                          model with two inputs with 4D and 2D shapes.
    --scale SCALE, -s SCALE
                          All input values coming from original network inputs
                          will be divided by this value. When a list of inputs
                          is overridden by the --input parameter, this scale is
                          not applied for any input that does not match with the
                          original input of the model.
    --reverse_input_channels
                          Switch the input channels order from RGB to BGR (or
                          vice versa). Applied to original inputs of the model
                          if and only if a number of channels equals 3. Applied
                          after application of --mean_values and --scale_values
                          options, so numbers in --mean_values and
                          --scale_values go in the order of channels used in the
                          original model.
    --log_level {CRITICAL,ERROR,WARN,WARNING,INFO,DEBUG,NOTSET}
                          Logger level
    --input INPUT         The name of the input operation of the given model.
                          Usually this is a name of the input placeholder of the
                          model.
    --output OUTPUT       The name of the output operation of the model. For
                          TensorFlow*, do not add :0 to this name.
    --mean_values MEAN_VALUES, -ms MEAN_VALUES
                          Mean values to be used for the input image per
                          channel. Values to be provided in the (R,G,B) or
                          [R,G,B] format. Can be defined for desired input of
                          the model, for example: "--mean_values
                          data[255,255,255],info[255,255,255]". The exact
                          meaning and order of channels depend on how the
                          original model was trained.
    --scale_values SCALE_VALUES
                          Scale values to be used for the input image per
                          channel. Values are provided in the (R,G,B) or [R,G,B]
                          format. Can be defined for desired input of the model,
                          for example: "--scale_values
                          data[255,255,255],info[255,255,255]". The exact
                          meaning and order of channels depend on how the
                          original model was trained.
    --data_type {FP16,FP32,half,float}
                          Data type for all intermediate tensors and weights. If
                          original model is in FP32 and --data_type=FP16 is
                          specified, all model weights and biases are quantized
                          to FP16.
    --disable_fusing      Turn off fusing of linear operations to Convolution
    --disable_resnet_optimization
                          Turn off resnet optimization
    --finegrain_fusing FINEGRAIN_FUSING
                          Regex for layers/operations that won't be fused.
                          Example: --finegrain_fusing Convolution1,.*Scale.*
    --disable_gfusing     Turn off fusing of grouped convolutions
    --move_to_preprocess  Move mean values to IR preprocess section
    --extensions EXTENSIONS
                          Directory or a comma separated list of directories
                          with extensions. To disable all extensions including
                          those that are placed at the default location, pass an
                          empty string.
    --batch BATCH, -b BATCH
                          Input batch size
    --version             Version of Model Optimizer
    --silent              Prevent any output messages except those that
                          correspond to log level equals ERROR, that can be set
                          with the following option: --log_level. By default,
                          log level is already ERROR.
    --freeze_placeholder_with_value FREEZE_PLACEHOLDER_WITH_VALUE
                          Replaces input layer with constant node with provided
                          value, e.g.: "node_name->True"
    --generate_deprecated_IR_V2
                          Force to generate legacy/deprecated IR V2 to work with
                          previous versions of the Inference Engine. The
                          resulting IR may or may not be correctly loaded by
                          Inference Engine API (including the most recent and
                          old versions of Inference Engine) and provided as a
                          partially-validated backup option for specific
                          deployment scenarios. Use it at your own discretion.
                          By default, without this option, the Model Optimizer
                          generates IR V3.
  
  TensorFlow*-specific parameters:
    --input_model_is_text
                          TensorFlow*: treat the input model file as a text
                          protobuf format. If not specified, the Model Optimizer
                          treats it as a binary file by default.
    --input_checkpoint INPUT_CHECKPOINT
                          TensorFlow*: variables file to load.
    --input_meta_graph INPUT_META_GRAPH
                          Tensorflow*: a file with a meta-graph of the model
                          before freezing
    --saved_model_dir SAVED_MODEL_DIR
                          TensorFlow*: directory representing non frozen model
    --saved_model_tags SAVED_MODEL_TAGS
                          Group of tag(s) of the MetaGraphDef to load, in string
                          format, separated by ','. For tag-set contains
                          multiple tags, all tags must be passed in.
    --offload_unsupported_operations_to_tf
                          TensorFlow*: automatically offload unsupported
                          operations to TensorFlow*
    --tensorflow_subgraph_patterns TENSORFLOW_SUBGRAPH_PATTERNS
                          TensorFlow*: a list of comma separated patterns that
                          will be applied to TensorFlow* node names to infer a
                          part of the graph using TensorFlow*.
    --tensorflow_operation_patterns TENSORFLOW_OPERATION_PATTERNS
                          TensorFlow*: a list of comma separated patterns that
                          will be applied to TensorFlow* node type (ops) to
                          infer these operations using TensorFlow*.
    --tensorflow_custom_operations_config_update TENSORFLOW_CUSTOM_OPERATIONS_CONFIG_UPDATE
                          TensorFlow*: update the configuration file with node
                          name patterns with input/output nodes information.
    --tensorflow_use_custom_operations_config TENSORFLOW_USE_CUSTOM_OPERATIONS_CONFIG
                          TensorFlow*: use the configuration file with custom
                          operation description.
    --tensorflow_object_detection_api_pipeline_config TENSORFLOW_OBJECT_DETECTION_API_PIPELINE_CONFIG
                          TensorFlow*: path to the pipeline configuration file
                          used to generate model created with help of Object
                          Detection API.
    --tensorboard_logdir TENSORBOARD_LOGDIR
                          TensorFlow*: dump the input graph to a given directory
                          that should be used with TensorBoard.
    --tensorflow_custom_layer_libraries TENSORFLOW_CUSTOM_LAYER_LIBRARIES
                          TensorFlow*: comma separated list of shared libraries
                          with TensorFlow* custom operations implementation.
    --disable_nhwc_to_nchw
                          Disables default translation from NHWC to NCHW
  ```

* 在这里比较值得注意的参数： 

  ```shell
  --input_meta_graph 输入的图：cnn_model.ckpt.meta
  --output_dir 输出的 .xml, .bin 文件输出的位置
  --input  输入节点名: x-input
  --output 输出节点名: layer8-fc2/Softmax
  --input_shape 输入矩阵的大小: [1,28,28,1] (注意不能有空格，使用 placeholder 时需要定义该参数)
  ```

* 注意要点

  * 由于 movidius 本身功能就是有局限的，在编译的过程中**某些 layer 可能不支持**

  * OpenVINO 在编译的过程中需要将模型 “冻结”，即将模型固定下来，因此 tensorflow 中使用了 placeholder 占位符的，需要指定输入块的大小，比如我的 x-input，在 tensorflow 中使用 **x=tf.placeholder(tf.float32, shape=[None, 28, 28, 1], name="x-input")**，这里的 None 就是一个占位符，但是**需要在使用 OpenVINO “冻结”模型时，指定输入的大小，我这里指定的是一次输入一个实例， 即 --input_shape [1,28,28,1]**

  * 至于在自己定义的输入节点名和输出节点名的获取，可以使用 tensorboard 对图进行可视化（**使用 tf.summary.FileWriter(...) 输出日志文件**），我是输出到 ./logs/ 文件夹中，然后再 ./logs/ 文件夹中打开终端输入 tensorboard --logdir=logs，然后打开在终端中出现的链接 http://ubuntu:6006，在浏览器中打开即可看到 tensorboard 的可视化界面。

    <div align="center">
        <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210301152906985.png" width=700>
    </div>

  * 比如在 tensorboard 可视化网页界面中， 我的输入节点和输出节点分别为：

    <div align="center">
        <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210301152933799.png" width=700>
        <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210301152946575.png" width=700>
    </div>

  * 在官方给出的文档中，有三种将 “未冻结” 的 tensorflow 模型进行编译的方法（[[参考链接]](https://software.intel.com/en-us/articles/OpenVINO-Using-TensorFlow)），但是本人还是觉得使用 .meta 进行优化编译的方式最为简便

    <div align="center">
        <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210301153052305.png" width=700>
    </div>

### 2.2 Model Optimizer 编译并优化模型

* 在终端中输入，这里**./model/cnn_model.ckpt.meta**是保存的模型路径：

  ```shell
  mo_tf.py --input_meta_graph ./model/cnn_model.ckpt.meta --output_dir model/ --input x-input --output layer8-fc2/Softmax --input_shape [1,28,28,1]
  ```

* 执行成功之后，如图所示：

  <div align="center">
      <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210301153205954.png" width=700>
  </div>

* 并在输出模型的 ./model/ 文件夹下，多了三个文件：

  <div align="center">
      <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210301153314000.png" width=700>
  </div>

## 三、将模型部署到 Movidius 并运行

* 进入 **~/inference_engine_samples/intel64/Release**，在该文件夹下有一个 **classification_sample** 进行模型推算

  <div align="center">
      <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/4a885732-9340-4a52-84d8-6fa34e685d1c.png" width=700>
  </div>

* 查看 ./classification_sample 所有参数：

  <div align="center">
      <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210301153526510.png" width=700>
  </div>

  * 同样的，这些参数也有几个值得重点关照的：
    * -i （必要参数）一个包含图片的文件夹路径 或者 一张图片的路径
    * -m (必要参数)  已经训练好的模型，即编译出来的 .xml 路径
    * -d 运行的设备，可以指定为 CPU, GPU, FPGA, MRIAD(神经棒)，当不指定时，默认为 CPU 运行

* 在 CPU 上运行（i5-8350HQ @3.7GHz）

  <div align="center">
      <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210301153937927.png" width=700>
  </div>

* 在 VPU 上运行（Neural Compute Stick 2，MYRIAD），参数接一个 **-d MYRIAD** 即可

  * 运行中出现了 VPU 不支持 FP32 的数据格式错误，后来查看了批文件后发现，**默认的 data_type 是 FP32**

    <div align="center">
        <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210301154000956.png" width=700>
    </div>

  * 因此重新使用 mo_tf.py 优化模型，设置参数 **--data_type FP16**

    <div align="center">
        <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210301154115567.png" width=700>
    </div>

  * 重新使用神经棒部署模型并运行

    <div align="center">
        <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210301154134468.png" width=700>
    </div>

## 四、执行结果

* 我们使用自己的 tensorflow 模型，编译为 .xml, .bin 后可以运行在 CPU 和 MRIAD，可以看出来，神经棒的推理速度还是很棒的！
* 但是输入的是一张 4 的图片，识别结果却是 1，而且识别率并不高，这是因为：对于 `classification_sample` 这个可执行文件，是针对于识别 demo 中的车的种类而特别编写的“样本例程”，并不是对所有的模型都适用。针对这个问题，我们将会在下一个总结笔记中，试用 **openvino.inference_egine** 中的组件，部署神经网络模型到 openvino 中进行识别