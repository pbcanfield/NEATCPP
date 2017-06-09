#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork()
{
     instructions = new Genome();
}
NeuralNetwork::NeuralNetwork(std::string dir)
{
     instructions = new Genome(dir);
}

NeuralNetwork::~NeuralNetwork()
{
     for(auto & node : input)
          delete node;

     for(auto & vec: hiddenLayer)
          for(auto & node : vec)
               delete node;

     for(auto & node : output)
          delete node;

     delete instructions;
}


void NeuralNetwork::updateStructure()
{
     input.clear();
     hiddenLayer.clear();
     output.clear();

     std::vector<unsigned int> topo = instructions -> getHidden();
     for(unsigned int i = 0; i < instructions -> getInput(); ++i)
          input.push_back(new Node());

     size_t size = topo.size();
     hiddenLayer.resize(size);
     for(size_t i = 0; i < size; ++i)
          for(unsigned int j = 0; j < topo[i]; ++i)
               hiddenLayer[i].push_back(new Node());
     for(unsigned int i = 0; i < instructions -> getOutput(); ++i)
          output.push_back(new Node());

}

void NeuralNetwork::loadFromFile(std::string dir)
{
      instructions -> loadFromFile(dir);
      updateStructure();
}

void NeuralNetwork::saveNetwork(std::string dir)
{
     instructions -> saveGenome(dir);
}


void NeuralNetwork::runForward()
{

}

void NeuralNetwork::batchGradientDescent(double stepSize)
{

}
