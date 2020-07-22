from sklearn import tree

classifier = tree.DecisionTreeClassifier(splitter='random',max_depth=3)

iris_clf = classifier.fit(x_train,y_train)

print(iris_clf.__class__)

y_predict = iris_clf.predict(x_test)


from sklearn.tree.export import export_text
# 文字化呈現
print(export_text(iris_clf,feature_names=iris.feature_names))

# plot the result
import matplotlib.pyplot as plt
plt.figure(figsize=(12,9))

# Decision Tree 特殊方法
tree.plot_tree(iris_clf,filled=True,class_names=iris.target_names,feature_names=iris.feature_names)
plt.show()